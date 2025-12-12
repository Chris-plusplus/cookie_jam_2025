#pragma once

#include <archimedes/Mmath.h>

namespace vs {

// Component which caches position before physics update
struct PrePhysicsPos {
	float2 value = {};
};

}