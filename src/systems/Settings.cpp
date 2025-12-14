#include <systems/Settings.h>
#include <components/Pledge.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>

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
	}
	{
		auto PlusMusic = scene.newEntity();
		auto&& PlusMusicTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Plus.png"));
		auto plusMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {PlusMusicTexture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		PlusMusic.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.59, windowHeight * 0.62, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {PlusMusicTexture->getWidth(), PlusMusicTexture->getHeight(), 0}
			}
		);
		PlusMusic.addComponent(
			scene::components::MeshComponent{
				.mesh = plusMesh,
				.pipeline = pipeline,
			}
		);
	}
	{
		auto PlusSFX = scene.newEntity();
		auto&& PlusSFXTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Plus.png"));
		auto plusSMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {PlusSFXTexture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		PlusSFX.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.59, windowHeight * 0.29, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {PlusSFXTexture->getWidth(), PlusSFXTexture->getHeight(), 0}
			}
		);
		PlusSFX.addComponent(
			scene::components::MeshComponent{
				.mesh = plusSMesh,
				.pipeline = pipeline,
			}
		);
	}
	{
		auto MinusMusic = scene.newEntity();
		auto&& MinusMusicTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Minus.png"));
		auto minusMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {MinusMusicTexture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		MinusMusic.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.41, windowHeight * 0.62, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {MinusMusicTexture->getWidth(), MinusMusicTexture->getHeight(), 0}
			}
		);
		MinusMusic.addComponent(
			scene::components::MeshComponent{
				.mesh = minusMesh,
				.pipeline = pipeline,
			}
		);
	}
	{
		auto MinusSFX = scene.newEntity();
		auto&& MinusSFXTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_Minus.png"));
		auto minusSMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {MinusSFXTexture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		MinusSFX.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.41, windowHeight * 0.29, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {MinusSFXTexture->getWidth(), MinusSFXTexture->getHeight(), 0}
			}
		);
		MinusSFX.addComponent(
			scene::components::MeshComponent{
				.mesh = minusSMesh,
				.pipeline = pipeline,
			}
		);
	}
	{
		auto MarkerM = scene.newEntity();
		auto&& MarkerMTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_znacznik.png"));
		auto MarkerMMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {MarkerMTexture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		MarkerM.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.5, windowHeight * 0.61, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {MarkerMTexture->getWidth(), MarkerMTexture->getHeight(), 0}
			}
		);
		MarkerM.addComponent(
			scene::components::MeshComponent{
				.mesh = MarkerMMesh,
				.pipeline = pipeline,
			}
		);
	}
	{
		auto MarkerS = scene.newEntity();
		auto&& MarkerSTexture = makeTexture(std::string_view("textures/Asset_final/Dzwiek/Dzwiek_znacznik.png"));
		auto MarkerSMesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {MarkerSTexture},
				.buffers = {defaultUniformBuffer()},
			}
			);
		MarkerS.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth * 0.5, windowHeight * 0.28, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {MarkerSTexture->getWidth(), MarkerSTexture->getHeight(), 0}
			}
		);
		MarkerS.addComponent(
			scene::components::MeshComponent{
				.mesh = MarkerSMesh,
				.pipeline = pipeline,
			}
		);
	}
}

void Settings::update(Scene& scene) {

}

void Settings::exit(Scene& scene) {
	auto temp = scene.domain().view<Settings>().front();
	scene.domain().kill(temp);
}
