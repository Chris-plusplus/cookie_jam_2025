#pragma once

#include <vector>
#include <archimedes/Mmath.h>

// LayerSystem needs LayerMatrix::get() to be defined
#ifdef LAYER_SYSTEM_DEFINED
#error "LayerSystem included before LayerMatrix"
#endif

using namespace arch;

namespace coll {

// layer collision matrix, simmilar to Unity's, but compile-time
struct LayerMatrix {
	// returns the actual layer matrix, defined in LayerMatrixDef.h
	static constexpr std::vector<std::vector<u32>> get();

	// checks if layers collide
	static consteval bool collides(u32 l1, u32 l2);
	// returns layer count
	static consteval u32 layerCount();
};

}

#include "LayerMatrix.hpp"