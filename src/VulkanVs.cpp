#include <VulkanVs.h>
#include <MakeTexture.h>
#include <archimedes/Font.h>
#include <archimedes/Scene.h>
#include <systems/Kill.h>
#include <Config.h>
#include <SoundManager.h>
#include <archimedes/text/TextComponent.h>
#include <Defaults.h>
#include <systems/StaticText.h>
#include <systems/MultilineText.h>
#include <systems/SlotMachine.h>
#include <MakeMesh.h>
#include <systems/PledgeSystem.h>
#include <systems/Button.h>

#include "PointsCounter.h"
#include "demon/OfferSystem.h"
#include "lifes/LifeManagerSystem.h"
#include "demon/DemonManager.h"
#include "demon/demon1.h"
#include "demon/demon2.h"
#include "demon/demon3.h"
#include "demon/SwitchSystem.h"
#include "demon/NegativeSwitch.h"
#include "demon/PositiveSwitch.h"
#include "slots/SlotGlitchChance.h"
#include "Scenes.h"
#include <systems/EndingSystem.h>
#include <button/Button.h>
#include <systems/Settings.h>

#include "systems/Autors.h"

#include "sound/Ambient.h"
#include "sound/AudioDomain.h"

ecs::Entity textEntity = ecs::nullEntity;
decltype(std::chrono::high_resolution_clock::now()) now{};

Unique<ecs::Domain> globalDomain;
Ref<Scene> mainScene{};
Ref<Scene> settingsScene{};
Ref<Scene> endingScene{};
Ref<Scene> menuScene{};
Ref<Scene> cutsceneScene{};
Ref<Scene> tutorialScene{};

VulkanVs::~VulkanVs() {
	mainScene = nullptr;
	settingsScene = nullptr;
	endingScene = nullptr;
	menuScene = nullptr;
	cutsceneScene = nullptr;
	tutorialScene = nullptr;
}

void VulkanVs::init() {
	// load font
	font::FontDB::get()["Arial"]->bold()->assure();
	font::FontDB::get()["Arial"]->regular()->assure();

	// make scene
	Ref<Scene> scene = createRef<Scene>();
	mainScene = scene;

	/*auto textEnt = scene->newEntity();
	textEntity = textEnt.handle();
	textEnt.addComponent(
		scene::components::TransformComponent{
			.position = {100, 500, -0.3},
			.rotation = {0, 0, 0, 1},
			.scale = {100, 100, 0}
		}
	);*/
	/*MultilineTextSystem::setup(*scene, textEntity, U"lorem\nipsum\ndupa", *font::FontDB::get()["Arial"]->regular(), {
		"shaders/text/fragment_atlas.glsl",
		"shaders/text/fragment_atlas_yellow.glsl",
		"shaders/text/fragment_atlas_blue.glsl"
		});*/


	audioDomain.global<SoundManager>().init();

	Ambient::setAmbient("menu_theme.ogg");

	//now = std::chrono::high_resolution_clock::now();

	PointsCounter::setup(*scene);

	demon1::setup(*scene);
	demon2::setup(*scene);
	demon3::setup(*scene);

	DemonManager::setup(*scene);

	demon::OfferSystem::setup(*scene);

	//SwitchSystem::addEffect<PositiveSwitch>(*scene, 1);
	//scene->domain().global<SlotGlitchChance>().value = 0.1;

	SlotMachineSystem::setup(*scene);

	LifeManagerSystem::setup(*scene);

	SlotMachineSystem::onDrawn(*scene, PointsCounter::count);

	settingsScene = createRef<Scene>();
	auto setting = settingsScene->newEntity();
	setting.addComponent(
		scene::components::TransformComponent{
			.position = {500, 500, -0.1},
			.rotation = {0, 0, 0, 1},
			.scale = {100, 100, 0}
		}
	);
	auto setTex = makeTexture("textures/slotMachine.png");
	setting.addComponent(
		scene::components::MeshComponent{
			.mesh = makeMesh(defaultCenterVertices(), defaultIndices()),
			.pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {setTex},
					.buffers = {defaultUniformBuffer()},
				}
				)
		}
	);

	scene = createRef<Scene>();
	menuScene = scene;
	scene::SceneManager::get()->changeScene(scene);

	auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
	{ // menu scene
		{ // logo
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Tytul_on_fin.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight * 5.f / 6.f, -0.1},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0} / 1.5f
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
		}
		{ // start
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Start_fin.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			auto&& t = entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth * 1 / 5, windowHeight * 2.f / 6.f, -0.1},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0} / 1.5f
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
			entity.addComponent(
				Button{
					.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
					.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
					.callback = [&](...) { scene::SceneManager::get()->changeScene(cutsceneScene); }
				}
			);
			// auto del = entity.addComponent(Button::InactiveFlag{});
		}
		{ // opcje
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Opcje_fin.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			auto&& t = entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth * 2 / 5, windowHeight * 2.f / 6.f, -0.1},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0} / 1.5f
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
			entity.addComponent(
				Button{
					.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
					.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
					.callback = [&](Scene& scene, ecs::Entity entity) mutable {
				for (auto&& button : scene.domain().view<Button>()) {
					scene.domain().addComponent<Button::InactiveFlag>(button);
				}
				Settings::setup(scene);
				// Entity(scene, entity).addComponent<Button::InactiveFlag>();
			}
				}
			);
		}
		{ // tworcy
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Tworcy_fin.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			auto&& t = entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth * 3 / 5, windowHeight * 2.f / 6.f, -0.1},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0} / 1.5f
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
			entity.addComponent(
				Button{
					.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
					.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
					.callback = [&](Scene& scene, ecs::Entity entity) mutable {
				for (auto&& button : scene.domain().view<Button>()) {
					scene.domain().addComponent<Button::InactiveFlag>(button);
				}
				Autors::setup(*menuScene);
			}
				}
			);
		}
		{ // wyjdz
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Wyjdz_fin.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			auto&& t = entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth * 4 / 5, windowHeight * 2.f / 6.f, -0.1},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0} / 1.5f
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
			entity.addComponent(
				Button{
					.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
					.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
					.callback = [&](...) { std::exit(0); }
				}
			);
		}
	}

	{ // cutscene
		cutsceneScene = createRef<Scene>();
		scene = cutsceneScene;

		{ // panel4
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Panel_4.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight / 2.f, -0.1},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0}
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
		}
		{ // panel3
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Panel_3.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight / 2.f, -0.2},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0}
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
		}
		{ // panel2
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Panel_2.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight / 2.f, -0.3},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0}
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
		}
		{ // panel1
			auto entity = scene->newEntity();
			auto tex = makeTexture("textures/Asset_final/Panel_1.png");
			auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
				gfx::pipeline::Pipeline::Desc{
					.vertexShaderPath = "shaders/vertex_default.glsl",
					.fragmentShaderPath = "shaders/fragment_default.glsl",
					.textures = {tex},
					.buffers = {defaultUniformBuffer()},
				}
				);
			entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight / 2.f, -0.4},
					.rotation = {0, 0, 0, 1},
					.scale = float3{tex->getWidth(), tex->getHeight(), 0}
				}
			);
			entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = pipeline
				}
			);
		}
	}
	{ // tutorial scene
		tutorialScene = createRef<Scene>();
		scene = tutorialScene;

		{ // panel
			auto entity = scene->newEntity();

			auto&& texVec = scene->domain().global<std::vector<Ref<gfx::pipeline::Pipeline>>>();

			auto makePipeline = [&](Ref<gfx::Texture> tex) {
				return gfx::Renderer::getCurrent()->getPipelineManager()->create(
					gfx::pipeline::Pipeline::Desc{
						.vertexShaderPath = "shaders/vertex_default.glsl",
						.fragmentShaderPath = "shaders/fragment_default.glsl",
						.textures = {tex},
						.buffers = {defaultUniformBuffer()},
					}
					);
			};

			auto celTex = makeTexture("textures/Asset_final/Tutorial/Tutorial_celgry.png");
			texVec.push_back(makePipeline(celTex));
			texVec.push_back(makePipeline(makeTexture("textures/Asset_final/Tutorial/Tutorial_sterowanie.png")));
			texVec.push_back(makePipeline(makeTexture("textures/Asset_final/Tutorial/Tutorial_mechanizm.png")));
			texVec.push_back(makePipeline(makeTexture("textures/Asset_final/Tutorial/Tutorial_nagrody.png")));
			texVec.push_back(makePipeline(makeTexture("textures/Asset_final/Tutorial/Tutorial_demony.png")));
			texVec.push_back(makePipeline(makeTexture("textures/Asset_final/Tutorial/Tutorial_zycie.png")));

			entity.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight / 2.f, -0.4},
					.rotation = {0, 0, 0, 1},
					.scale = float3{celTex->getWidth(), celTex->getHeight(), 0} / 1.5f
				}
			);
			auto&& meshC = entity.addComponent(
				scene::components::MeshComponent{
					.mesh = mesh,
					.pipeline = texVec[0]
				}
			);

			Color transparent{};
			auto transparentPip = makePipeline(gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &transparent));
			auto hoverTex = makeTexture("textures/Asset_final/Tutorial/Tutorial_button_hover.png");
			auto hoverPip = makePipeline(hoverTex);

			auto file = std::ifstream("tutorialButtons.txt");
			float buttonScale{};
			float2 buttonSize = {225.f, 70.f};
			file >> buttonScale;
			{ // button1
				auto button = scene->newEntity();
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{hoverTex->getWidth(), hoverTex->getHeight(), 0} *buttonScale
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = transparentPip
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonSize.x / 2.f, buttonSize.y / 2.f},
						.bottomRight = {buttonSize.x / 2.f, -buttonSize.y / 2.f},
						.onNotHover = transparentPip,
						.onHover = hoverPip,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { meshCPtr->pipeline = texVecPtr->at(0);  }
					}
				);
			}
			{ // button2
				auto button = scene->newEntity();
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{hoverTex->getWidth(), hoverTex->getHeight(), 0}*buttonScale
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = transparentPip
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonSize.x / 2.f, buttonSize.y / 2.f},
						.bottomRight = {buttonSize.x / 2.f, -buttonSize.y / 2.f},
						.onNotHover = transparentPip,
						.onHover = hoverPip,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { meshCPtr->pipeline = texVecPtr->at(1);  }
					}
				);
			}
			{ // button3
				auto button = scene->newEntity();
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{hoverTex->getWidth(), hoverTex->getHeight(), 0}*buttonScale
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = transparentPip
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonSize.x / 2.f, buttonSize.y / 2.f},
						.bottomRight = {buttonSize.x / 2.f, -buttonSize.y / 2.f},
						.onNotHover = transparentPip,
						.onHover = hoverPip,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { meshCPtr->pipeline = texVecPtr->at(2);  }
					}
				);
			}
			{ // button4
				auto button = scene->newEntity();
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{hoverTex->getWidth(), hoverTex->getHeight(), 0}*buttonScale
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = transparentPip
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonSize.x / 2.f, buttonSize.y / 2.f},
						.bottomRight = {buttonSize.x / 2.f, -buttonSize.y / 2.f},
						.onNotHover = transparentPip,
						.onHover = hoverPip,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { meshCPtr->pipeline = texVecPtr->at(3);  }
					}
				);
			}
			{ // button5
				auto button = scene->newEntity();
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{hoverTex->getWidth(), hoverTex->getHeight(), 0}*buttonScale
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = transparentPip
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonSize.x / 2.f, buttonSize.y / 2.f},
						.bottomRight = {buttonSize.x / 2.f, -buttonSize.y / 2.f},
						.onNotHover = transparentPip,
						.onHover = hoverPip,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { meshCPtr->pipeline = texVecPtr->at(4);  }
					}
				);
			}
			{ // button6
				auto button = scene->newEntity();
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{hoverTex->getWidth(), hoverTex->getHeight(), 0}*buttonScale
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = transparentPip
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonSize.x / 2.f, buttonSize.y / 2.f},
						.bottomRight = {buttonSize.x / 2.f, -buttonSize.y / 2.f},
						.onNotHover = transparentPip,
						.onHover = hoverPip,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { meshCPtr->pipeline = texVecPtr->at(5);  }
					}
				);
			}

			{ // mainScene button
				auto button = scene->newEntity();
				auto buttonTex = makeTexture("textures/Asset_final/Zamyknie_asset.png");
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.3},
						.rotation = {0, 0, 0, 1},
						.scale = float3{buttonTex->getWidth(), buttonTex->getHeight(), 0}
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				auto pipeline = makePipeline(buttonTex);
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = pipeline
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonT.scale.x / 2.f, buttonT.scale.y / 2.f},
						.bottomRight = {buttonT.scale.x / 2.f, -buttonT.scale.y / 2.f},
						.onNotHover = pipeline,
						.onHover = pipeline,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { scene::SceneManager::get()->changeScene(mainScene); std::ofstream("watchedTutorial") << ""; }
					}
				);
			}

			{ // mainScene -> tutorial button
				auto button = mainScene->newEntity();
				auto buttonTex = makeTexture("textures/Asset_final/Button_tutorial.png");
				auto buttonHoverTex = makeTexture("textures/Asset_final/Button_tutorial_hover.png");
				auto&& buttonT = button.addComponent(
					scene::components::TransformComponent{
						.position = {0, 0, -0.475},
						.rotation = {0, 0, 0, 1},
						.scale = float3{buttonTex->getWidth(), buttonTex->getHeight(), 0}
					}
				);
				file >> buttonT.position.x >> buttonT.position.y;
				auto pipeline = makePipeline(buttonTex);
				auto pipelineHover = makePipeline(buttonHoverTex);
				button.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = pipeline
					}
				);
				button.addComponent(
					Button{
						.topLeft = {-buttonT.scale.x / 2.f, buttonT.scale.y / 2.f},
						.bottomRight = {buttonT.scale.x / 2.f, -buttonT.scale.y / 2.f},
						.onNotHover = pipeline,
						.onHover = pipelineHover,
						.callback = [&, meshCPtr = &meshC, texVecPtr = &texVec](...) { scene::SceneManager::get()->changeScene(tutorialScene); }
					}
				);
			}
		}
	}
}

using namespace std::chrono_literals;

void VulkanVs::update() {
	/*if (input::Keyboard::enter.pressed()) {
		EndingSystem::end("textures/Asset_final/Bad_ending.png");
	}*/
	Settings::update(*menuScene);

	Ref<Scene> scene = scene::SceneManager::get()->currentScene();
	ButtonSystem::update(*scene);

	audioDomain.global<SoundManager>().audioManager->synchronize(scene->domain());

	if (scene == mainScene) {
		//static auto prevTime = std::chrono::high_resolution_clock::now();


		SlotMachineSystem::update(*scene);
		LifeManagerSystem::update(*scene);
		PointsCounter::update(*scene);

		PledgeSystem::update(*scene);
		//if (scene == mainScene) {
		DemonManager::update(*scene);
	//}

	// synchronize audio

		/*if (std::chrono::high_resolution_clock::now() - now > std::chrono::seconds(3)) {
			MultilineTextSystem::remove(*scene, textEntity);
		}*/
	} else if (scene == cutsceneScene) {
		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::high_resolution_clock::now();

		if (currTime - startTime >= std::chrono::milliseconds(5000) || input::Keyboard::space.pressed()) {
			startTime = currTime;
			scene->domain().kill(scene->domain().view<scene::components::TransformComponent>().back());
			if (scene->domain().components<scene::components::TransformComponent>().base().count() == 0) {
				scene::SceneManager::get()->changeScene(std::filesystem::exists("watchedTutorial") ? mainScene : tutorialScene);
				Ambient::stopAmbient();
				Ambient::setAmbient("main_theme.ogg");

			}
		}
	}

}

