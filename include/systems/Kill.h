#pragma once

#include <archimedes/Scene.h>

using namespace arch;

namespace vs {

// Kills all entities with KillFlag
struct KillSystem {
	static void update(Scene& scene);
};

}