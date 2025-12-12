#pragma once

#include <archimedes/Scene.h>

using namespace arch;

namespace vs {

// Updates Transforms' positions from Movables
struct TransformUpdateSystem {
	static void update(ecs::Domain& domain);
};

}