#include <systems/Explosion.h>
#include <random>
#include <Config.h>
#include <Defaults.h>
#include <archimedes/Font.h>
#include <archimedes/Text.h>
#include <components/BoomTextTime.h>
#include <components/Vulkan.h>
#include <components/Kill.h>
#include <systems/Particle.h>
#include <SoundManager.h>
#include <archimedes/physics/components/Moveable.h>
#include <chrono>

namespace vs {

void ExplosionSystem::makeText(Scene& scene) {
	// settings
	static auto rng = std::mt19937(std::random_device{}());
	static auto fontSizeDist = std::uniform_real_distribution(75.f, 150.f);
	static auto angleDist = std::uniform_real_distribution(-75.f, 75.f);
	static const auto margin = 200.f;
	static auto xDist = std::uniform_real_distribution(margin, windowWidth - margin);
	static auto yDist = std::uniform_real_distribution(margin, windowHeight - margin);

	auto boomText = scene.newEntity();
	boomText.addComponent(
		scene::components::TransformComponent{
			.position = float3{xDist(rng), yDist(rng), -0.2f},
			.rotation = glm::angleAxis(angleDist(rng), zAxis()) * glm::quat(0, 0, 0, 1),
			.scale = fontSizeDist(rng) * float3 { 1, 1, 0 }
		}
	);
	boomText.addComponent(
		text::TextComponent(U"BUM!", {defaultUniformBuffer()}, *font::FontDB::get()["Arial"]->bold())
	);
	boomText.addComponent<BoomTextTime>(std::chrono::high_resolution_clock::now());
}

void ExplosionSystem::updateText(ecs::Domain& domain) {
	auto now = std::chrono::high_resolution_clock::now();
	// updates time
	domain.view<const text::TextComponent, const BoomTextTime>().forEach([&domain, &now](ecs::Entity entity, const BoomTextTime& time) {
		if (now - time.value >= std::chrono::seconds(1)) {
			domain.addComponent<Kill>(entity);
		}
	});
}

void ExplosionSystem::makeExplosion(Scene& scene, Entity vulkan) {
	auto&& vulkanComp = vulkan.getComponent<Vulkan>();

	for (u32 i = 0; i != vulkanComp.particleCount; ++i) {
		ParticleSystem::setup(scene, vulkan.addChild(), vulkanComp);
	}

	makeText(scene);

	auto&& source = vulkan.getComponent<audio::AudioSourceComponent>();
	auto&& audioManager = *scene.domain().global<SoundManager>().audioManager;
	audioManager.rewindSource(source);
	audioManager.playSource(source);
}

void ExplosionSystem::setupListener(Scene& scene, Entity vulkan1, Entity vulkan2) {
	auto center = (vulkan1.getComponent<Vulkan>().particleOrigin + vulkan2.getComponent<Vulkan>().particleOrigin) / 2.f;

	auto listener = scene.newEntity();
	auto&& listenerComp = listener.addComponent<audio::ListenerComponent>();

	scene.domain().global<SoundManager>().audioManager->setListener(scene.domain(), listenerComp);
}

}