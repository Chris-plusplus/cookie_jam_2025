#include <systems/Settings.h>
#include <components/Pledge.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <systems/SettingsFlag.h>
#include <systems/Button.h>
#include <button/Button.h>

#include "sound/Ambient.h"
#include "sound/SFX.h"

int Settings::Music=50;
int Settings::SFX=50;
scene::components::TransformComponent* Music_poz;
scene::components::TransformComponent* SFX_poz;

void Settings::setup(Scene& scene) {
	if (scene.domain().components<Settings>().base().count() != 0) {
		return;
	}
	{
		auto Settings = scene.newEntity();
		auto &&containerTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_asset.png"));
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {containerTexture},
				.buffers = {defaultUniformBuffer()},
			}
		);
		Settings.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, windowHeight / 2, -0.8},
				.rotation = {0, 0, 0, 1},
				.scale = {containerTexture->getWidth() * 0.8, containerTexture->getHeight() * 0.8, 0}
			}
		);
		Settings.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline,
			}
		);
		auto&& flag = Settings.addComponent(SettingsFlag{});
	}
	{
		auto PlusMusic = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Plus.png"));
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& t = PlusMusic.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.59, windowHeight * 0.62, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			}
		);
		PlusMusic.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline,
			}
		);
		PlusMusic.addComponent(
				Button{
					.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
					.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
					.callback = [&](...) { Music=std::clamp(Music+10, 0, 100); }
				});
		auto&& flag = PlusMusic.addComponent(SettingsFlag{});
	}
	{
		auto PlusSFX = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Plus.png"));
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& t = PlusSFX.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.59, windowHeight * 0.29, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			}
		);
		PlusSFX.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline,
			}
		);
		PlusSFX.addComponent(
			Button{
				.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
				.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
				.callback = [&](...) { SFX=std::clamp(SFX+10, 0, 100); }
			});
		auto&& flag = PlusSFX.addComponent(SettingsFlag{});
	}
	{
		auto MinusMusic = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Minus.png"));
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& t = MinusMusic.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.41, windowHeight * 0.62, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			}
		);
		MinusMusic.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline,
			}
		);
		MinusMusic.addComponent(
				Button{
					.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
					.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
					.callback = [&](...) { Music=std::clamp(Music-10, 0, 100); }
				});
		auto&& flag = MinusMusic.addComponent(SettingsFlag{});
	}
	{
		auto MinusSFX = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Minus.png"));
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& t = MinusSFX.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.41, windowHeight * 0.29, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			}
		);
		MinusSFX.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline,
			}
		);
		MinusSFX.addComponent(
			Button{
				.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
				.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
				.callback = [&](...) { SFX=std::clamp(SFX-10, 0, 100); }
			});
		auto&& flag = MinusSFX.addComponent(SettingsFlag{});
	}
	{
		auto MarkerM = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_znacznik.png"));
		auto MarkerMMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		Music_poz = &MarkerM.addComponent(scene::components::TransformComponent{
				.position = {windowWidth * 0.5, windowHeight * 0.61, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			});
		MarkerM.addComponent(
			scene::components::MeshComponent{
				.mesh = MarkerMMesh,
				.pipeline = pipeline,
			}
		);
		auto&& flag = MarkerM.addComponent(SettingsFlag{});
	}
	{
		auto MarkerS = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_znacznik.png"));
		auto MarkerSMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		SFX_poz = &MarkerS.addComponent(scene::components::TransformComponent{
				.position = {windowWidth * 0.5, windowHeight * 0.28, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			});
		MarkerS.addComponent(
			scene::components::MeshComponent{
				.mesh = MarkerSMesh,
				.pipeline = pipeline,
			}
		);
		auto&& flag = MarkerS.addComponent(SettingsFlag{});
	}
	{
		auto Exit = scene.newEntity();
		auto&& texture = makeTexture(std::string_view("textures/Asset_final/Zamyknie_asset.png"));
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {texture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& t = Exit.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.6, windowHeight * 0.85, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {texture->getWidth(), texture->getHeight(), 0}
			}
		);
		Exit.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline,
			}
		);
		Exit.addComponent(
			Button{
				.topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
				.bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
				.callback = [&](...) { exit(scene); }
			});
		auto&& flag = Exit.addComponent(SettingsFlag{});
	}
}

void Settings::update(Scene& scene) {
	if (scene.domain().view<SettingsFlag>()) {
		SFX_poz->position.x=windowWidth * (0.45+0.001*SFX);
		Music_poz->position.x=windowWidth * (0.45+0.001*Music);
	}
	SFX::gain = (float) SFX / 100.0;
	Ambient::gain = (float) Music / 100.0;
	Ambient::updateGain();
}

void Settings::exit(Scene& scene) {
	std::vector<ecs::Entity> toKill;
	for (auto&& entity : scene.domain().view<SettingsFlag>()) {
		toKill.push_back(entity);
	}
	for (auto&& entity : toKill) {
		scene.domain().kill(entity);
	}
	std::vector<ecs::Entity> buttonsToActivate;
	for (auto&& entity : scene.domain().view<Button::InactiveFlag>()) {
		buttonsToActivate.push_back(entity);
	}
	for (auto&& entity : buttonsToActivate) {
		scene.domain().removeComponent<Button::InactiveFlag>(entity);
	}
}
