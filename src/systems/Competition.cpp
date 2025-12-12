#include <systems/Competition.h>
#include <components/Competition.h>
#include <components/Particle.h>
#include <Defaults.h>
#include <Config.h>
#include <archimedes/Text.h>
#include <archimedes/Font.h>

namespace vs {

void CompetitionSystem::setup(Scene& scene) {
	auto subSetup = [&scene]<bool IsRight>(std::bool_constant<IsRight>) {
		// max score display
		auto max = scene.newEntity();
		auto&& maxTransform = max.addComponent(
			scene::components::TransformComponent{
				.position = {10.f + IsRight * (windowWidth / 2.f), windowHeight - 100.f, -0.2f},
				.rotation = {0, 0, 0, 1},
				.scale = {100, 100, 0}
			}
		);
		max.addComponent<CompetitionMaxParticles<IsRight>>();

		// current score display
		auto curr = scene.newEntity();
		auto&& currTransform = curr.addComponent<scene::components::TransformComponent>() = maxTransform;
		currTransform.position.y -= 100;
		curr.addComponent<CompetitionCurrentTextFlag<IsRight>>();
	};
	subSetup(std::true_type{});
	subSetup(std::false_type{});
}

void CompetitionSystem::update(Scene& scene) {
	if (input::Keyboard::esc.down()) {
		// reset scores
		scene.domain().components<CompetitionMaxParticles<true>>().front().value = 0;
		scene.domain().components<CompetitionMaxParticles<false>>().front().value = 0;
	}

	auto subUpdate = [&scene]<bool IsRight>(std::bool_constant<IsRight>) {
		auto&& [_maxEntity, max] = scene.domain().view<CompetitionMaxParticles<IsRight>>().all().front();
		auto maxEntity = Entity(scene, _maxEntity);

		auto buffer = defaultUniformBuffer();

		// display current score
		auto curr = scene.entitiesWith<CompetitionCurrentTextFlag<IsRight>>().front();
		auto currParticles = scene.domain().components<Particle<IsRight>>().base().count();
		curr.removeComponent<text::TextComponent>();
		curr.addComponent(
			text::TextComponent(
				text::convertTo<char32_t>(std::string_view(std::format("Current: {}", currParticles))),
				{buffer},
				"Arial"
			)
		);

		// display max score
		max.value = std::max(max.value, currParticles);
		maxEntity.removeComponent<text::TextComponent>();
		maxEntity.addComponent(
			text::TextComponent(
				text::convertTo<char32_t>(std::string_view(std::format("Max: {}", max.value))),
				{buffer},
				"Arial"
			)
		);
	};
	subUpdate(std::true_type{});
	subUpdate(std::false_type{});
}

}