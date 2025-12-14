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

ecs::Entity textEntity = ecs::nullEntity;
decltype(std::chrono::high_resolution_clock::now()) now{};

Unique<ecs::Domain> globalDomain;
Ref<Scene> mainScene;
Ref<Scene> settingsScene;

VulkanVs::~VulkanVs() {
	mainScene = nullptr;
	settingsScene = nullptr;
}

void VulkanVs::init() {
	// load font
	font::FontDB::get()["Arial"]->bold()->assure();
	font::FontDB::get()["Arial"]->regular()->assure();

	// make scene
	Ref<Scene> scene = createRef<Scene>();
	mainScene = scene;
	scene::SceneManager::get()->changeScene(scene);

	auto textEnt = scene->newEntity();
	textEntity = textEnt.handle();
	textEnt.addComponent(
		scene::components::TransformComponent{
			.position = {100, 500, -0.3},
			.rotation = {0, 0, 0, 1},
			.scale = {100, 100, 0}
		}
	);
	MultilineTextSystem::setup(*scene, textEntity, U"lorem\nipsum\ndupa", *font::FontDB::get()["Arial"]->regular(), {
		"shaders/text/fragment_atlas.glsl",
		"shaders/text/fragment_atlas_yellow.glsl",
		"shaders/text/fragment_atlas_blue.glsl"
		});

	// init SoundManager
	scene->domain().global<SoundManager>().init({explosionSoundPath});

	now = std::chrono::high_resolution_clock::now();

	PointsCounter::setup(*scene);

	demon1::setup(*scene);
	demon2::setup(*scene);
	demon3::setup(*scene);

	DemonManager::setup(*scene);

	SwitchSystem::addEffect<PositiveSwitch>(*scene, 1);
	scene->domain().global<SlotGlitchChance>().value = 0.5;

	SlotMachineSystem::setup(*scene);

	LifeManagerSystem::setup(*scene);

	demon::OfferSystem::setup(*scene);
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
}

using namespace std::chrono_literals;

void VulkanVs::update() {
	if (input::Keyboard::enter.pressed()) {
		if (scene::SceneManager::get()->currentScene() == mainScene) {
			scene::SceneManager::get()->changeScene(settingsScene);
		} else {
			scene::SceneManager::get()->changeScene(mainScene);
		}
	}

	Ref<Scene> scene = scene::SceneManager::get()->currentScene();
	if (scene == mainScene) {
		//static auto prevTime = std::chrono::high_resolution_clock::now();

		ButtonSystem::update(*scene);

		PointsCounter::update(*scene);

		SlotMachineSystem::update(*scene);
		LifeManagerSystem::update(*scene);

		if (input::Keyboard::Q.pressed()) {
			PledgeSystem::setup(*scene);
			PledgeSystem::setCallback(*scene, [&] {
				Logger::debug("anim end");
			});
		}

		PledgeSystem::update(*scene);
		DemonManager::update(*scene);

	// synchronize audio
		scene->domain().global<SoundManager>().audioManager->synchronize(scene->domain());

		/*if (std::chrono::high_resolution_clock::now() - now > std::chrono::seconds(3)) {
			MultilineTextSystem::remove(*scene, textEntity);
		}*/
	}
}

