#include <systems/Particle.h>
#include <components/Particle.h>
#include <Defaults.h>
#include <archimedes/physics/System.h>
#include <archimedes/physics/components/Moveable.h>
#include <components/colliders/LayerCollisionFlag.h>
#include <components/colliders/LayeredAABB.h>
#include <Config.h>
#include <components/Kill.h>

namespace vs {

void ParticleSystem::setup(Scene& scene, Entity particle, Vulkan& vulkan) {
	static auto rng = std::mt19937(std::random_device{}());
	// initial rotation angle distribution
	static auto initAngleDist = std::uniform_real_distribution(0.f, 360.f);

	const float size = vulkan.sizeDist(rng);
	auto&& transform = particle.addComponent(
		scene::components::TransformComponent{
			.position = vulkan.particleOrigin + float3{0, 0, -0.1f},
			.rotation = glm::angleAxis(initAngleDist(rng), zAxis()),
			.scale = float3{size, size, 0}
		}
	);
	particle.addComponent(
		scene::components::MeshComponent{
			.mesh = vulkan.particleMesh,
			.pipeline = vulkan.particlePipeline
		}
	);

	if (vulkan.isRight) {
		particle.addComponent<Particle<true>>() = {
			.rotationQuat = glm::angleAxis(vulkan.rotationSpeedDist(rng), zAxis()),
			.e = std::sqrt(vulkan.eDist(rng))
		};
	} else {
		particle.addComponent<Particle<false>>() = {
			.rotationQuat = glm::angleAxis(vulkan.rotationSpeedDist(rng), zAxis()),
			.e = std::sqrt(vulkan.eDist(rng))
		};
	}

	const auto angle = vulkan.explosionAngleDist(rng);
	const auto vel = vulkan.velocityDist(rng);

	particle.addComponent(
		physics::Moveable{
			.center = {.mass = 1.f, .position = transform.position},
			.force = float2{0, -9.81f * 100},
			.velocity = vel * float2{-sin(angle), cos(angle)}
		}
	);
	auto&& aabb = particle.addComponent<coll::LayeredAABB<Layer::particle>>();
	aabb.topLeft = float3{-size, size, 0} / 2.f;
	aabb.bottomRight = float3{size, -size, 0} / 2.f;
}

inline void particleCollisions(ecs::Domain& domain);

void ParticleSystem::update(ecs::Domain& domain) {
	// rotate with angular velocity
	auto logic = [&domain]<bool IsRight>(std::bool_constant<IsRight>) {
		auto view = domain.view<scene::components::TransformComponent, const Particle<IsRight>>();
		for (auto&& [transform, particle] : view.components()) {
			transform.rotation = particle.rotationQuat * transform.rotation;
		}
	};
	logic(std::true_type{});
	logic(std::false_type{});

	particleCollisions(domain);
}

void particleCollisions(ecs::Domain& domain) {
	std::vector<ecs::Entity> toRemoveCollision;

	// handle collisions with ground
	auto logic = [&domain, &toRemoveCollision] <bool IsRight>(std::bool_constant<IsRight>) {
		for (auto&& [entity, particle, movable, aabb, t] : domain.view<
			Particle<IsRight>,
			LayerCollisionFlag<Layer::ground>,
			physics::Moveable,
			coll::LayeredAABB<Layer::particle>,
			scene::components::TransformComponent>().all()
			) {
			++particle.collisionCounter;
			if (particle.collisionCounter >= 3) {
				domain.addComponent<Kill>(entity);
				continue;
			}

			const auto insideDist = -(movable.center.position.y + aabb.bottomRight.y);

			movable.velocity.x *= particle.e;
			movable.velocity.y *= -particle.e;

			t.position.y = movable.center.position.y += insideDist;
		}
	};
	logic(std::true_type{});
	logic(std::false_type{});
}


}