#include <systems/Ground.h>
#include <Config.h>
#include <Defaults.h>
#include <components/colliders/LayeredAABB.h>

namespace vs {

// creates ground collider
void GroundSystem::setup(Scene& scene) {
	auto ground = scene.newEntity();

	auto&& aabb = ground.addComponent<coll::LayeredAABB<Layer::ground>>();
	aabb.topLeft = float2{-1000000, 0};
	aabb.bottomRight = float2{1000000, -1000};

	ground.addComponent(
		scene::components::TransformComponent{
			.position = float3{windowWidth / 2, 0, 0},
			.rotation = {0, 0, 0, 1},
			.scale = {1, 1, 0}
		}
	);
}

}