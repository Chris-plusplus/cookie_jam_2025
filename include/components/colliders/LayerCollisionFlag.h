#pragma once

#include <archimedes/ArchMath.h>

using namespace arch;


// Flag indicating that the owner collider with layer L
template<u32 L>
struct LayerCollisionFlag {
	static constexpr u32 layer = L;
	static constexpr bool flagComponent = true;
};

