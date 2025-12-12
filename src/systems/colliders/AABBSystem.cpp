#include <systems/colliders/AABBSystem.h>
#include <components/colliders/LayeredAABB.h>
#include <components/colliders/PrePhysicsPos.h>
#include <archimedes/physics/components/Moveable.h>
#include <LayerMatrixDef.h>

namespace coll {

// simple rect collision
bool AABBSystem::areColliding(const AABB& collider1, const AABB& collider2, const float2 t1Pos, const float2 t2Pos) {
	AABB c1 = collider1;
	c1.topLeft += t1Pos;
	c1.bottomRight += t1Pos;

	AABB c2 = collider2;
	c2.topLeft += t2Pos;
	c2.bottomRight += t2Pos;

	return c1.topLeft.x <= c2.bottomRight.x && c1.bottomRight.x >= c2.topLeft.x &&
		c1.topLeft.y >= c2.bottomRight.y && c1.bottomRight.y <= c2.topLeft.y;
}

}