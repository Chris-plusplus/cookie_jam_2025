#include <systems/EndingSystem.h>
#include <archimedes/Gfx.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <Scenes.h>
#include <archimedes/Text.h>
#include <PointsCounter.h>

#include "SoundManager.h"
#include "sound/Ambient.h"
#include "sound/AudioDomain.h"

using namespace arch;

void EndingSystem::end(Scene& scene, std::string_view texturePath, std::string_view endingTheme) {
	Ambient::stopAmbient();
	Ambient::setAmbient(endingTheme.data());
	endingScene = createRef<Scene>();
	scene::SceneManager::get()->changeScene(endingScene);

	auto ending = endingScene->newEntity();
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
			.position = {windowWidth / 2, windowHeight / 2, -0.1},
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

	auto score = endingScene->newEntity();
	auto&& textT = score.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2, windowHeight / 2, -0.2},
			.rotation = {0, 0, 0, 1},
			.scale = {200, 200, 0}
		}
	);
	auto&& textComp = score.addComponent(
		text::TextComponent(
			text::convertTo<char32_t>(std::string_view(std::format("{}", PointsCounter::score))),
			{defaultUniformBuffer()},
			"Pixelated Elegance",
			"Regular",
			"shaders/text/fragment_atlas_outline.glsl"
		)
	);

	auto bottomLeft = textComp.bottomLeftAdjusted();
	auto topRight = textComp.topRight();

	auto diff = (topRight - bottomLeft) / 2.f;
	diff.x *= -textT.scale.x;
	diff.y *= -textT.scale.y;

	Logger::debug("diff = {} {}", diff.x, diff.y);

	textT.position -= diff;
}