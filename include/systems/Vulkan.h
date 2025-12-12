#pragma once

#include <archimedes/Scene.h>

using namespace arch;

namespace vs {

// volcano behavior
struct VulkanSystem {
	static void setup(Entity vulkan, input::Key& key, bool isRight);
	static void update(Scene& scene);
};

}