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

namespace demon {
void OfferSystem::spawnOfferDialogue(Scene& scene) {
	auto&& manager = scene.domain().view<OfferDialogue>().front();
	auto&& offerDialogue = scene.domain().getComponent<OfferDialogue>(manager);

	auto&& renderer = *gfx::Renderer::current();

	// prepare container texture and params
	auto&& containerTexture = makeTexture(std::string_view("textures/Asset_szkice/Okienko_dialog.png"));
	offerDialogue.containerWidth = containerTexture->getWidth();
	offerDialogue.containerHeight = containerTexture->getHeight();
	offerDialogue.containerPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(containerTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);
	offerDialogue.containerX = offerDialogue.containerWidth / 2;
	offerDialogue.containerY = windowHeight - offerDialogue.containerHeight / 2;
	offerDialogue.containerScaleX = 0.9;
	offerDialogue.containerScaleY = 0.9;

	// prepare accept button texture and params
	auto&& acceptTexture = makeTexture(std::string_view("textures/Asset_szkice/Akceptacja_button.png"));
	offerDialogue.buttonWidth = acceptTexture->getWidth();
	offerDialogue.buttonHeight = acceptTexture->getHeight();
	offerDialogue.acceptButtonPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(acceptTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);

		// prepare dismiss button texture and params
	auto&& dismissTexture = makeTexture(std::string_view("textures/Asset_szkice/Odrzucenie_button.png"));
	offerDialogue.dismissButtonPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(dismissTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);


		// add accept button to the screen
	offerDialogue.acceptButtonX = offerDialogue.buttonWidth / 2;
	offerDialogue.acceptButtonY = windowHeight - offerDialogue.buttonHeight / 2;
	offerDialogue.buttonScaleX = 0.9;
	offerDialogue.buttonScaleY = 0.9;

	auto&& accept = scene.newEntity();
	accept.addComponent(
		scene::components::TransformComponent{
			.position = {offerDialogue.acceptButtonX, offerDialogue.acceptButtonY, -0.7},
			.rotation = {0, 0, 0, 1},
			.scale = {offerDialogue.buttonWidth * offerDialogue.buttonScaleX, offerDialogue.buttonHeight * offerDialogue.buttonScaleY, 0}
		}
	);
	auto mesh1 = makeMesh(defaultCenterVertices(), defaultIndices());
	accept.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh1,
			.pipeline = offerDialogue.acceptButtonPipeline
		}
	);
	accept.addComponent(AcceptButtonFlag{});

	// add dismiss button texture to the screen
	offerDialogue.dismissButtonX = offerDialogue.buttonWidth;
	offerDialogue.dismissButtonY = windowHeight - offerDialogue.buttonHeight;

	auto&& dismiss = scene.newEntity();
	dismiss.addComponent(
		scene::components::TransformComponent{
			.position = {offerDialogue.dismissButtonX, offerDialogue.dismissButtonY, -0.7},
			.rotation = {0, 0, 0, 1},
			.scale = {offerDialogue.buttonWidth * offerDialogue.buttonScaleX, offerDialogue.buttonHeight * offerDialogue.buttonScaleY, 0}
		}
	);
	auto mesh2 = makeMesh(defaultCenterVertices(), defaultIndices());
	dismiss.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh2,
			.pipeline = offerDialogue.dismissButtonPipeline
		}
	);
	dismiss.addComponent(DismissButtonFlag{});

	// add container button texture to the screen
	auto&& container = scene.newEntity();
	auto&& containerT = container.addComponent(
		scene::components::TransformComponent{
			.position = {offerDialogue.containerX, offerDialogue.containerY, -0.7},
			.rotation = {0, 0, 0, 1},
			.scale = {offerDialogue.containerWidth * offerDialogue.containerScaleX, offerDialogue.containerHeight * offerDialogue.containerScaleY, 0}
		}
	);
	auto mesh3 = makeMesh(defaultCenterVertices(), defaultIndices());
	container.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh3,
			.pipeline = offerDialogue.containerPipeline
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

void OfferSystem::setup(Scene& scene) {
	auto&& manager = scene.newEntity();
	auto&& offerDialogue = manager.addComponent(OfferDialogue{});
	spawnOfferDialogue(scene);
}

void OfferSystem::update(Scene& scene) {}

}
