#include <demon/OfferSystem.h>
#include <Defaults.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <fstream>
#include <archimedes/Input.h>
#include "MakeTexture.h"
#include "demon/OfferDialogue.h"
#include "demon/AcceptButtonFlag.h"
#include "demon/DismissButtonFlag.h"
#include "demon/ContainerFlag.h"
#include <systems/MultilineText.h>
#include <archimedes/Font.h>
#include <archimedes/Text.h>
#include <systems/Button.h>

namespace demon {
void OfferSystem::setup(Scene& scene) {
	auto&& manager = scene.newEntity();
	auto&& dial = manager.addComponent(OfferDialogue{});
	auto&& renderer = *gfx::Renderer::current();

	// prepare container texture and params
	auto&& containerTexture = makeTexture(std::string_view("textures/Asset_final/okienko_dialog.png"));
	dial.containerWidth = containerTexture->getWidth();
	dial.containerHeight = containerTexture->getHeight();
	dial.containerPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(containerTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);
	dial.containerScaleX = 0.9;
	dial.containerScaleY = 0.9;
	dial.containerX = windowWidth / 2;
	dial.containerY = windowHeight - dial.containerHeight * dial.containerScaleY / 2;


	// prepare accept button texture and params
	auto&& acceptTexture = makeTexture(std::string_view("textures/Asset_final/Akceptacja_button.png"));
	dial.buttonWidth = acceptTexture->getWidth();
	dial.buttonHeight = acceptTexture->getHeight();
	dial.acceptButtonPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(acceptTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);

	// prepare dismiss button texture and params
	auto&& dismissTexture = makeTexture(std::string_view("textures/Asset_final/Odrzucenie_button.png"));
	dial.dismissButtonPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(dismissTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);

	dial.buttonScaleX = 0.9;
	dial.buttonScaleY = 0.9;
	// add accept button to the screen
	dial.acceptButtonX = dial.containerX + dial.containerWidth / 2 * dial.containerScaleX - 1.55 * dial.buttonWidth * dial.buttonScaleX;
	dial.acceptButtonY = dial.containerY - dial.containerHeight / 2 * dial.containerScaleY + 1.75 * dial.buttonHeight * dial.buttonScaleY;


	auto&& accept = scene.newEntity();
	auto&& acceptT = accept.addComponent(
		scene::components::TransformComponent{
			.position = {dial.acceptButtonX, dial.acceptButtonY, 1.0},
			.rotation = {0, 0, 0, 1},
			.scale = {dial.buttonWidth * dial.buttonScaleX, dial.buttonHeight * dial.buttonScaleY, 0}
		}
	);
	auto mesh1 = makeMesh(defaultCenterVertices(), defaultIndices());
	accept.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh1,
			.pipeline = dial.acceptButtonPipeline
		}
	);
	ButtonSystem::setup(scene, accept.handle(), float2{-acceptT.scale.x, acceptT.scale.y} / 2.f, float2{acceptT.scale.x, -acceptT.scale.y} / 2.f, [&](...) {
		clearOfferDialogue(scene);
	});
	accept.addComponent(AcceptButtonFlag{});

	// add dismiss button texture to the screen
	dial.dismissButtonX = dial.acceptButtonX - dial.buttonWidth * dial.buttonScaleX;
	dial.dismissButtonY = dial.acceptButtonY;

	auto&& dismiss = scene.newEntity();
	auto&& dismissT = dismiss.addComponent(
		scene::components::TransformComponent{
			.position = {dial.dismissButtonX, dial.dismissButtonY,  1.0},
			.rotation = {0, 0, 0, 1},
			.scale = {dial.buttonWidth * dial.buttonScaleX, dial.buttonHeight * dial.buttonScaleY, 0}
		}
	);
	auto mesh2 = makeMesh(defaultCenterVertices(), defaultIndices());
	dismiss.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh2,
			.pipeline = dial.dismissButtonPipeline
		}
	);
	ButtonSystem::setup(scene, dismiss.handle(), float2{-dismissT.scale.x, dismissT.scale.y} / 2.f, float2{dismissT.scale.x, -dismissT.scale.y} / 2.f, [&](...) {
		clearOfferDialogue(scene);
	});
	dismiss.addComponent(DismissButtonFlag{});

	// add container button texture to the screen
	auto&& container = scene.newEntity();
	auto&& containerT = container.addComponent(
		scene::components::TransformComponent{
			.position = {dial.containerX, dial.containerY, 1.0},
			.rotation = {0, 0, 0, 1},
			.scale = {dial.containerWidth * dial.containerScaleX, dial.containerHeight * dial.containerScaleY, 0}
		}
	);
	auto mesh3 = makeMesh(defaultCenterVertices(), defaultIndices());
	container.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh3,
			.pipeline = dial.containerPipeline
		}
	);
	container.addComponent(ContainerFlag{});

	{ // example text
		auto textParent = scene.newEntity();
		float3 textDeltaPos;
		float fontSize;
		{
			auto file = std::ifstream("offerConfig.txt");
			file >> textDeltaPos.x >> textDeltaPos.y >> textDeltaPos.z >> fontSize;
		}
		textParent.addComponent(
			scene::components::TransformComponent{
				.position = containerT.position + float3{-containerT.scale.x, containerT.scale.y, 0} / 2.f + textDeltaPos,
				.rotation = {0, 0, 0, 1},
				.scale = {fontSize, fontSize, 0}
			}
		);

		std::u32string offerText;
		{
			auto file = std::ifstream("offer.txt");
			file.seekg(0, std::ios::end);
			size_t size = file.tellg();
			std::string buffer(size, ' ');
			file.seekg(0);
			file.read(&buffer[0], size);

			offerText = text::convertTo<char32_t>(std::string_view(buffer));
		}
		MultilineTextSystem::setup(scene, textParent, offerText, *font::FontDB::get()["Arial"]->regular());
	}
}

void OfferSystem::clearOfferDialogue(Scene &scene) {
	auto&& dial = scene.domain().view<ContainerFlag>().front();
	auto&& transform = scene.domain().getComponent<scene::components::TransformComponent>(dial);
	transform.position.z = 1.0;

	auto&& accept = scene.domain().view<AcceptButtonFlag>().front();
	auto&& transform2 = scene.domain().getComponent<scene::components::TransformComponent>(accept);
	transform2.position.z = 1.0;

	auto&& dismiss = scene.domain().view<DismissButtonFlag>().front();
	auto&& transform3 = scene.domain().getComponent<scene::components::TransformComponent>(dismiss);
	transform3.position.z = 1.0;
}

void OfferSystem::spawnOfferDialogue(Scene& scene) {
	auto&& dial = scene.domain().view<ContainerFlag>().front();
	auto&& transform = scene.domain().getComponent<scene::components::TransformComponent>(dial);
	transform.position.z = -0.65;

	auto&& accept = scene.domain().view<AcceptButtonFlag>().front();
	auto&& transform2 = scene.domain().getComponent<scene::components::TransformComponent>(accept);
	transform2.position.z = -0.7;

	auto&& dismiss = scene.domain().view<DismissButtonFlag>().front();
	auto&& transform3 = scene.domain().getComponent<scene::components::TransformComponent>(dismiss);
	transform3.position.z = -0.7;
}

void OfferSystem::update(Scene& scene) {}

}
