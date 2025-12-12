#include <MakeMesh.h>


Ref<asset::mesh::Mesh> makeMesh(std::span<Vertex>&& vs, std::span<u32>&& is) {
	auto bufferManager = gfx::Renderer::getCurrent()->getBufferManager();
	return asset::mesh::Mesh::create(
		bufferManager->createVertexBuffer(std::move(vs)),
		bufferManager->createIndexBuffer(std::move(is))
	);
}

