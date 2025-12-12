#pragma once

#include <Vertex.h>
#include <archimedes/gfx/Renderer.h>

using namespace arch;

namespace vs {

// returns default uniform buffer, containing orthographic projection matrix
Ref<gfx::buffer::Buffer> defaultUniformBuffer();

// returns default vertices, for displaying sprites
std::vector<Vertex>& defaultVertices();
// returns vertices for particles (makes rotations easier)
std::vector<Vertex>& defaultParticleVertices();

// returns default indices, for displaying sprites
std::vector<u32>& defaultIndices();

// returns Z axis for rotations
constexpr float3 zAxis() {
	return float3{0, 0, 1};
}

}