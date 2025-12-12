#pragma once

#include <archimedes/asset/mesh/Mesh.h>
#include <Vertex.h>

using namespace arch;

namespace vs {

// creates mesh from vertices and indices
Ref<asset::mesh::Mesh> makeMesh(std::span<Vertex>&& vs, std::span<u32>&& is);

}