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

#include "demon/deals.h"
#include "demon/DemonManager.h"
#include "demon/OfferTextFlag.h"
#include "sound/Ambient.h"
#include "systems/PledgeSystem.h"
#include "lifes/LifeManager.h"
#include "systems/EndingSystem.h"

namespace demon {

OfferType OfferSystem::offer = OfferType::_none;

void OfferSystem::setup(Scene& scene) {
	auto&& manager = scene.newEntity();
	auto&& dial = manager.addComponent<OfferDialogue>();
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

	auto&& acceptHoverTexture = makeTexture(std::string_view("textures/Asset_final/Akceptacja_button_hover.png"));
	dial.acceptButtonHoverPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(acceptHoverTexture)},
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

	auto&& dismissHoverTexture = makeTexture(std::string_view("textures/Asset_final/Odrzucenie_button_hover.png"));
	dial.dismissButtonHoverPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(dismissHoverTexture)},
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
	ButtonSystem::setup(scene, accept.handle(), float2{-acceptT.scale.x, acceptT.scale.y} / 2.f, float2{acceptT.scale.x, -acceptT.scale.y} / 2.f, dial.acceptButtonPipeline, dial.acceptButtonHoverPipeline, [&](...) {
		PledgeSystem::setup(scene);
		PledgeSystem::setCallback(scene, [&] {
			DemonManager::hide(scene);
		});
		switch (offer) {
			case OfferType::d11:
				Deal::big_sachet(scene);
				Deal::nolife(scene);
				break;
			case OfferType::d12:
				Deal::big_sachet(scene);
				Deal::luck_down(scene);
				break;
			case OfferType::d13:
				Deal::set_positive_change(scene);
				Deal::nolife(scene);
				break;
			case OfferType::d14:
				Deal::set_positive_change(scene);
				Deal::luck_down(scene);
				break;
			case OfferType::d21:
				Deal::quality();
				Deal::robber();
				break;
			case OfferType::d22:
				Deal::quality();
				Deal::set_negative_change(scene);
				break;
			case OfferType::d31:
				Deal::set_glitch(scene);
				break;
			case OfferType::d32:
				Deal::set_no_gurken();
				Deal::short_on_life(scene);
				break;
			default:
				Logger::debug("Błąd oferty demona");
		}
	});
	accept.addComponent(AcceptButtonFlag{});

	// add dismiss button texture to the screen
	dial.dismissButtonX = dial.acceptButtonX - dial.buttonWidth * dial.buttonScaleX;
	dial.dismissButtonY = dial.acceptButtonY;

	auto&& dismiss = scene.newEntity();
	auto&& dismissT = dismiss.addComponent(
		scene::components::TransformComponent{
			.position = {dial.dismissButtonX, dial.dismissButtonY, 1.0},
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
	ButtonSystem::setup(scene, dismiss.handle(), float2{-dismissT.scale.x, dismissT.scale.y} / 2.f, float2{dismissT.scale.x, -dismissT.scale.y} / 2.f, dial.dismissButtonPipeline, dial.dismissButtonHoverPipeline, [&](...) {
		DemonManager::hide(scene);
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
}

void OfferSystem::clearOfferDialogue(Scene& scene) {
	auto&& dial = scene.domain().view<ContainerFlag>().front();
	auto&& transform = scene.domain().getComponent<scene::components::TransformComponent>(dial);
	transform.position.z = 1.0;

	auto&& accept = scene.domain().view<AcceptButtonFlag>().front();
	auto&& transform2 = scene.domain().getComponent<scene::components::TransformComponent>(accept);
	transform2.position.z = 1.0;

	auto&& dismiss = scene.domain().view<DismissButtonFlag>().front();
	auto&& transform3 = scene.domain().getComponent<scene::components::TransformComponent>(dismiss);
	transform3.position.z = 1.0;

	auto&& multiline = scene.domain().view<OfferTextFlag>().front();
	MultilineTextSystem::remove(scene, multiline);
	scene.domain().kill(multiline);

	Ambient::stopAmbient();
	Ambient::setAmbient("main_theme.ogg");

	if (scene.domain().components<LifeManager>().front().currentLifes == 0) {
		EndingSystem::end(scene, "textures/Asset_final/Bad_ending.png", "bad_ending_theme.ogg");
	}
}

void OfferSystem::spawnOfferDialogue(Scene& scene, std::string_view offerText, OfferType new_offer) {
	if (DemonManager::isBlocked) {
		return;
	}
	offer = new_offer;
	auto&& container = scene.domain().view<ContainerFlag>().front();
	auto&& containerT = scene.domain().getComponent<scene::components::TransformComponent>(container);
	containerT.position.z = -0.65;

	auto&& accept = scene.domain().view<AcceptButtonFlag>().front();
	auto&& acceptT = scene.domain().getComponent<scene::components::TransformComponent>(accept);
	acceptT.position.z = -0.7;

	auto&& dismiss = scene.domain().view<DismissButtonFlag>().front();
	auto&& dismissT = scene.domain().getComponent<scene::components::TransformComponent>(dismiss);
	dismissT.position.z = -0.7;

	{
		std::ofstream stream("offer.txt", std::ios::out | std::ios::trunc);
		stream << offerText;
	}

	{ // example text
		auto textParent = scene.newEntity();
		float3 textDeltaPos;
		float fontSize;
		{
			auto file = std::ifstream("offerConfig.txt");
			file >> textDeltaPos.x >> textDeltaPos.y >> textDeltaPos.z >> fontSize;
		}
		auto&& dial = scene.domain().view<OfferDialogue>().front();
		auto&& cnt = scene.domain().getComponent<OfferDialogue>(dial);

		auto&& textParentT = textParent.addComponent(
			scene::components::TransformComponent{
				.position = float3(cnt.containerX - cnt.containerWidth / 2 * cnt.containerScaleX,
					cnt.containerY + cnt.containerHeight / 2 * cnt.buttonScaleY,
					-0.8) + float3{-cnt.containerScaleX, cnt.containerScaleY, 0.0} / 2.f + textDeltaPos,
					.rotation = {0, 0, 0, 1},
					.scale = {fontSize, fontSize, 0}
			}
		);
		textParentT.position.z = -0.8;
		textParent.addComponent(OfferTextFlag{});

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
		Logger::debug("pre {}, {}, {}", textParentT.position.x, textParentT.position.y, textParentT.position.z);

		MultilineTextSystem::setup(scene, textParent, offerText, *font::FontDB::get()["Arial"]->regular(),
			{"shaders/text/fragment_atlas_black.glsl", });

		Ambient::stopAmbient();
		Ambient::setAmbient("demon_theme.ogg");
	}
}

void OfferSystem::update(Scene& scene) {}

}
