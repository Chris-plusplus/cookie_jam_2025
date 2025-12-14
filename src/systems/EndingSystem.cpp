#include <systems/EndingSystem.h>
#include <archimedes/Gfx.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <Scenes.h>

#include "SoundManager.h"
#include "sound/Ambient.h"
#include "sound/AudioDomain.h"

using namespace arch;

void EndingSystem::end(Scene& scene, std::string_view texturePath, std::string_view endingTheme) {
	Ambient::stopAmbient();
	Ambient::setAmbient(endingTheme.data());
	// endingScene = createRef<Scene>();
	// scene::SceneManager::get()->changeScene(endingScene);

	auto ending = scene.newEntity();
	auto endingTex = makeTexture(texturePath);
	auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
	auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {endingTex},
			.buffers = {defaultUniformBuffer()},
		}
		);
	ending.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2, windowHeight / 2, -1.0},
			.rotation = {0, 0, 0, 1},
			.scale = {endingTex->getWidth(), endingTex->getHeight(), 0}
		}
	);
	ending.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh,
			.pipeline = pipeline
		}
	);
}