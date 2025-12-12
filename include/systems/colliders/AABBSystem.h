#pragma once

#include <components/colliders/AABB.h>
#include <archimedes/scene/components/TransformComponent.h>
#include <archimedes/Ecs.h>

using namespace arch;

namespace coll {

// Behavior of AABB colliders
class AABBSystem {
public:
	// whether two colliders are colliding
	static bool areColliding(const AABB& collider1, const AABB& collider2, const float2 t1Pos, const float2 t2Pos);
};

}