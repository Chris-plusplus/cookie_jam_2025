#include <systems/EndingSystem.h>
#include <archimedes/Gfx.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <Scenes.h>
#include <archimedes/Text.h>
#include <PointsCounter.h>
#include <button/Button.h>
#include <systems/Button.h>
#include <systems/SlotMachine.h>

#include "SoundManager.h"
#include "sound/Ambient.h"
#include "sound/AudioDomain.h"

using namespace arch;

namespace {
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
}

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

	if (texturePath.contains("Good")) {
		auto oneMore = endingScene->newEntity();
		Logger::debug("one more {:v}", oneMore.handle());
		oneMore.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, windowHeight / 2, -0.9},
				.rotation = {0, 0, 0, 1},
				.scale = {windowWidth, windowHeight, 0}
			}
		);

		Color transparent = {0, 0, 0, 0};
		auto transparentTex = gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &transparent);
		auto transparentPipeline = makePipeline(transparentTex);
		oneMore.addComponent(
			scene::components::MeshComponent{
				.mesh = makeMesh(defaultCenterVertices(), defaultIndices()),
				.pipeline = transparentPipeline
			}
		);
		Color black = {0, 0, 0, 1};
		auto blTex = gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &black);
		auto blPipeline = makePipeline(blTex);
		oneMore.addComponent(
			Button{
				.topLeft = {-(int)windowWidth / 2, windowHeight / 2},
				.bottomRight = {windowWidth / 2, -(int)windowHeight / 2},
				.onNotHover = transparentPipeline,
				.onHover = transparentPipeline
			}
		).callback = [](Scene& scene, ecs::Entity _button) {
			auto button = Entity(scene, _button);
			Color color = {0, 0, 0, 1};
			auto blackTex = gfx::Renderer::getCurrent()->getTextureManager()->createTexture2D(1, 1, &color);
			auto blackPipeline = makePipeline(blackTex);
			auto&& buttonC = button.getComponent<Button>();
			buttonC.callback = [](Scene& scene, ecs::Entity _button) {
				scene::SceneManager::get()->changeScene(mainScene);
				SlotMachineSystem::isEnd = false;
				Ambient::stopAmbient();
				Ambient::setAmbient("main_theme.ogg");
			};
			buttonC.onHover = blackPipeline;
			buttonC.onNotHover = blackPipeline;
			button.getComponent<scene::components::MeshComponent>().pipeline = blackPipeline;

			auto text = scene.newEntity();
			auto&& textT = text.addComponent(
				scene::components::TransformComponent{
					.position = {windowWidth / 2, windowHeight / 2, -0.95},
					.rotation = {0, 0, 0, 1},
					.scale = {200, 200, 0}
				}
			);
			auto&& textComp = text.addComponent(
				text::TextComponent(
					U"Jeszcze jeden?",
					{defaultUniformBuffer()},
					"Arial"
				)
			);

			auto bottomLeft = textComp.bottomLeftAdjusted();
			auto topRight = textComp.topRight();

			auto diff = (topRight - bottomLeft) / 2.f;
			diff.x *= -textT.scale.x;
			diff.y *= -textT.scale.y;

			textT.position -= diff;
		};
	}
}