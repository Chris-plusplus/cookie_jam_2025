#pragma once

#include <random>
#include <archimedes/Scene.h>
#include <components/Vulkan.h>

using namespace arch;

namespace vs {

// particles behavior
struct ParticleSystem {
	static void setup(Scene& scene, Entity particle, Vulkan& vulkan);
	static void update(ecs::Domain& domain);
};

}