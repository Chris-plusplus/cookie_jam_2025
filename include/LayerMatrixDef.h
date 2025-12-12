#pragma once

// this file must be included before LayerSystem
#ifdef LAYER_SYSTEM_DEFINED
#error "LayerSystem included before LayerMatrix"
#endif

#include <systems/colliders/LayerMatrix.h>

namespace coll {

constexpr std::vector<std::vector<u32>> LayerMatrix::get() {
	return {
		{1, 0},	// 0 <-> 1, 0 <-> 0
		{0}		// 1 <-> 1
	};
}

}