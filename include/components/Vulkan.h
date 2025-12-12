#pragma once

#include <random>
#include <archimedes/Mmath.h>
#include <archimedes/Input.h>
#include <archimedes/gfx/Texture.h>
#include <archimedes/gfx/pipeline/Pipeline.h>
#include <archimedes/asset/mesh/Mesh.h>

using namespace arch;


// Volcano component
struct Vulkan {
	using FloatDist = std::uniform_real_distribution<float>;

	// how many particles per explosion
	u32 particleCount;
	// explosion key
	input::Key* key;

	// distribution of shootout of particles
	FloatDist explosionAngleDist;
	// distribution of initial particle velocity
	FloatDist velocityDist;
	// distribution of particle rotation speed
	FloatDist rotationSpeedDist;
	// distribution of particle size
	FloatDist sizeDist;
	// distribution of particle "bounceinness"
	FloatDist eDist;

	// reference to particle texture
	Ref<gfx::Texture> texture;
	// reference to particle pipeline
	Ref<gfx::pipeline::Pipeline> particlePipeline;
	// reference to particle mesh
	Ref<asset::mesh::Mesh> particleMesh;

	// place of explosion
	float3 particleOrigin;

	// whether the volcano is the one on the right
	bool isRight;
};

