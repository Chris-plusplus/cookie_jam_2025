#pragma once

#include "Explosion.h"
#include <array>
#include <archimedes/Input.h>

using namespace arch;

namespace vs {

// behavior for steering auto explosion mechanism
struct AutoExplosionSystem {
	static void setup(Entity vulkan, input::Key& key, std::vector<input::Key*> disablers = {});
	static void update(Scene& scene);
};

}