#pragma once

#include <components/colliders/LayeredAABB.h>

namespace vs::coll {

// behavior for layers
class LayerSystem {
public:
	static inline void update(ecs::Domain& domain);
	static inline void removeFlags(ecs::Domain& domain);
};

}

#include "LayerSystem.hpp"