#include "LayerSystem.h"

#define LAYER_SYSTEM_DEFINED

#include "LayerMatrix.h"
#include <tuple>
#include <archimedes/Ecs.h>
#include <components/colliders/LayeredAABB.h>
#include "AABBSystem.h"
#include <components/colliders/LayerCollisionFlag.h>
#include <systems/TransformUpdate.h>

namespace coll {

namespace detail {

// iterates through layer matrix, collumn-wise
template<u32 Row, u32 Col, u32 End>
void updateIterateCol(ecs::Domain& domain) {
	if constexpr (Col < End) {
		if constexpr (LayerMatrix::collides(Row, Col)) {
			// simple each-each collision
			// in practice: 1 ground and N particles
			for (auto&& [entity1, aabb1, t1] : domain.view<LayeredAABB<Row>, scene::components::TransformComponent>().all()) {
				for (auto&& [entity2, aabb2, t2] : domain.view<LayeredAABB<Col>, scene::components::TransformComponent>().all()) {
					if (AABBSystem::areColliding(aabb1, aabb2, t1.position, t2.position)) {
						domain.addComponent<LayerCollisionFlag<Col>>(entity1);
						domain.addComponent<LayerCollisionFlag<Row>>(entity2);
					}
				}
			}
		}
		updateIterateCol<Row, Col + 1, End>(domain);
	}
}

// iterates through layer matrix, row-wise
template<u32 Row, u32 End>
void updateIterateRow(ecs::Domain& domain) {
	if constexpr (Row < End) {
		// iterate over row
		updateIterateCol<Row, Row, End>(domain);
		// iterator over column
		updateIterateRow<Row + 1, End>(domain);
	}
}

// removes all layer collision flags
template<u32 I, u32 End>
void removeFlagsIterate(ecs::Domain& domain) {
	if constexpr (I < End) {
		// removing from behind saves on moves + view iterators don't get invalidated
		for (auto&& entity : domain.view<LayerCollisionFlag<I>>() | std::views::reverse) {
			domain.removeComponent<LayerCollisionFlag<I>>(entity);
		}
		removeFlagsIterate<I + 1, End>(domain);
	}
}

}

inline void LayerSystem::update(ecs::Domain& domain) {
	detail::updateIterateRow<0, LayerMatrix::layerCount()>(domain);
}

inline void LayerSystem::removeFlags(ecs::Domain& domain) {
	detail::removeFlagsIterate<0, LayerMatrix::layerCount()>(domain);
}

}