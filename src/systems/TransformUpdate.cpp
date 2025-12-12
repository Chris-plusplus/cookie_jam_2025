#include <systems/TransformUpdate.h>
#include <archimedes/physics/components/Moveable.h>

namespace vs {

// updates Transforms with Movables
void TransformUpdateSystem::update(ecs::Domain& domain) {
	for (auto&& [movable, transform] : domain.view<physics::Moveable, scene::components::TransformComponent>().components()) {
		transform.position = float3(movable.center.position, transform.position.z);
	}
}

}