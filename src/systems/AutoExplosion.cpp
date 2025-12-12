#include <systems/AutoExplosion.h>
#include <chrono>
#include <systems/Competition.h>
#include <archimedes/Input.h>
#include <components/Vulkan.h>

using namespace arch;


struct AutoExplosionData {
	bool working;
	decltype(std::chrono::high_resolution_clock::now()) lastTime;
	// toggles auto-explosion
	input::Key* key;
	// disables auto-explosion
	std::vector<input::Key*> disablers;
};

void AutoExplosionSystem::setup(Entity entity, input::Key& key, std::vector<input::Key*> disablers) {
	if (entity.hasComponent<Vulkan>()) {
		entity.addComponent(
			AutoExplosionData{
				.working = false,
				.lastTime = {},
				.key = &key,
				.disablers = std::move(disablers)
			}
		);
	}
}

void AutoExplosionSystem::update(Scene& scene) {
	const auto now = std::chrono::high_resolution_clock::now();

	for (auto&& [entity, autoExplosion, vulkan] : scene.domain().view<AutoExplosionData, Vulkan>().all()) {
		if (autoExplosion.key->pressed()) {
			autoExplosion.working = !autoExplosion.working;
			autoExplosion.lastTime = {};
		}

		for (auto&& key : autoExplosion.disablers) {
			if (key && key->pressed()) {
				autoExplosion.working = false;
				autoExplosion.lastTime = {};
				break;
			}
		}

		if (autoExplosion.working && now - autoExplosion.lastTime >= std::chrono::milliseconds(1500)) {
			ExplosionSystem::makeExplosion(scene, Entity(scene, entity));

			autoExplosion.lastTime = now;
		}
	}
}

