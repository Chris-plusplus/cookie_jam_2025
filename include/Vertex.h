#pragma once

#include <archimedes/Mmath.h>

using namespace arch;

namespace vs {

// type of vertices
struct Vertex {
	// position of vertex
	float3 position;
	// texture coordinates of vertex
	float2 texCoords;
};

}