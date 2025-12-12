#pragma once

#include <archimedes/Scene.h>

using namespace arch;

namespace vs {

// handles explosions
struct ExplosionSystem {
	static void makeText(Scene& scene);
	static void updateText(ecs::Domain& domain);
	static void makeExplosion(Scene& scene, Entity vulkan);

	static void setupListener(Scene& scene, Entity vulkan1, Entity vulkan2);
};

}