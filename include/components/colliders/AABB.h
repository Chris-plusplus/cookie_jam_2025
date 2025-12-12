#pragma once

#include <archimedes/ArchMath.h>

using namespace arch;

namespace coll {

// Bounding box
struct AABB {
	float2 topLeft;
	float2 bottomRight;
};

}