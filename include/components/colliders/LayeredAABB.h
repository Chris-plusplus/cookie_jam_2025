#pragma once

#include "AABB.h"

namespace coll {

// Bounding box assigned to a layer
template<u32 L>
struct LayeredAABB: public AABB {};

}