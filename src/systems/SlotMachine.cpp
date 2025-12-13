#include <components/SlotMachine.h>
#include <systems/SlotMachine.h>
#include <Defaults.h>
#include <slots/RewardGenerator.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <fstream>
#include <slots/SlotFlag.h>
#include <slots/SlotObject.h>
#include <archimedes/Input.h>
#include <cmath>
#include <numbers>

using namespace std::chrono_literals;

struct LeverFlag {
	static constexpr bool flagComponent = true;
};

struct Paw {
	float originalY;
	float movementScale;
};

void SlotMachineSystem::setup(Scene& scene) {
	auto machine = scene.newEntity();
	auto&& slotMachine = machine.addComponent<SlotMachine>();
	auto&& renderer = *gfx::Renderer::current();

	scene.domain().global<slots::RewardGenerator>();

	float2 slotTexSize{};
	// init slots' pipelines
	for (auto i = 0; i != (int)slots::RewardType::_count; ++i) {
		auto&& symbolPipeline = slotMachine.symbols.emplace_back();


		auto&& texture = makeTexture(std::string_view(std::format("textures/slots/{}.png", i)));
		slotTexSize.x = texture->getWidth();
		slotTexSize.y = texture->getHeight();
		symbolPipeline = renderer.getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {std::move(texture)},
				.buffers = {defaultUniformBuffer()},
			}
			);

	}

	// init machine sprite
	auto machineTexture = makeTexture("textures/slotMachine.png");
	auto&& transform = machine.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2, windowHeight / 2, -0.5},
			.rotation = {0, 0, 0, 1},
			.scale = float3{machineTexture->getWidth(), machineTexture->getHeight(), 0} *0.9f
		}
	);
	auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
	auto pipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(machineTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);
	auto&& meshComp = machine.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh,
			.pipeline = pipeline
		}
	);

	{ // init slots objects
		auto file = std::ifstream("slotConfig.txt");
		for (auto i = 0; i != slotMachine.slotCount; ++i) {
			float originalX{}, originalY{};
			file >> originalX >> originalY;

			slotMachine.lowerBound = originalY - 1.f * slotTexSize.y;
			slotMachine.upperBound = originalY + 2.f * slotTexSize.y;

			auto&& collumn = slotMachine.slotsByCollumn.emplace_back();
			for (auto j = -1; j <= 1; ++j) {
				auto slotEntity = scene.newEntity();
				collumn.push_back(slotEntity.handle());
				auto&& transform = slotEntity.addComponent(
					scene::components::TransformComponent{
						.position = {originalX, originalY + j * slotTexSize.y, -0.1},
						.rotation = {0, 0, 0, 1},
						.scale = float3(slotTexSize, 0)
					}
				);
				auto&& meshCompSlot = slotEntity.addComponent(
					scene::components::MeshComponent{
						.mesh = mesh,
						.pipeline = slotMachine.symbols[i]
					}
				);
				slotEntity.addComponent<SlotObject>();
			}
		}
	}

	{ // init lever
		auto lever = scene.newEntity();
		lever.addComponent<LeverFlag>();
		float3 leverPos{0, 0, -0.05};
		{
			auto file = std::ifstream("leverPos.txt");
			file >> leverPos.x >> leverPos.y;
		}
		auto&& leverTex = makeTexture("textures/lever.png");
		auto&& leverT = lever.addComponent(
			scene::components::TransformComponent{
				.position = leverPos,
				.rotation = {0, 0, 0, 1},
				.scale = float3(leverTex->getWidth(), leverTex->getHeight(), 0)
			}
		);
		auto&& leverMesh = lever.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = renderer.getPipelineManager()->create(
					gfx::pipeline::Pipeline::Desc{
						.vertexShaderPath = "shaders/vertex_default.glsl",
						.fragmentShaderPath = "shaders/fragment_default.glsl",
						.textures = {leverTex},
						.buffers = {defaultUniformBuffer()},
					}
					)
			}
		);
	}

	{ // init paw
		auto paw = scene.newEntity();
		float3 pawPos{0, 0, -0.6};
		{
			auto file = std::ifstream("pawPos.txt");
			auto&& pawComp = paw.addComponent<Paw>();
			file >> pawPos.x >> pawPos.y >> pawComp.movementScale;
			pawComp.originalY = pawPos.y;
		}
		auto&& pawTex = makeTexture("textures/paw.png");
		auto&& pawT = paw.addComponent(
			scene::components::TransformComponent{
				.position = pawPos,
				.rotation = {0, 0, 0, 1},
				.scale = float3(pawTex->getWidth(), pawTex->getHeight(), 0) / 1.4f
			}
		);
		auto&& pawMesh = paw.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = renderer.getPipelineManager()->create(
					gfx::pipeline::Pipeline::Desc{
						.vertexShaderPath = "shaders/vertex_default.glsl",
						.fragmentShaderPath = "shaders/fragment_default.glsl",
						.textures = {pawTex},
						.buffers = {defaultUniformBuffer()},
					}
					)
			}
		);
	}
}

void SlotMachineSystem::update(Scene& scene) {
	auto&& slotMachine = scene.domain().components<SlotMachine>().front();
	if (input::Keyboard::space.pressed() && slotMachine.leverAnimationSpeed == 0) {
		slotMachine.leverAnimationSpeed = 10;

		for (auto&& [slotObject] : scene.domain().view<SlotObject>().components()) {
			if (slotObject.jolt == 0) {
				slotObject.jolt = -10000.f;
			}
		}


	}
	updateAnimation(scene);
}

void SlotMachineSystem::updateAnimation(Scene& scene) {
	static auto prevTime = std::chrono::high_resolution_clock::now();
	const auto now = std::chrono::high_resolution_clock::now();
	auto deltaTime = (float)std::chrono::duration_cast<decltype(0.0s)>(now - prevTime).count();
	prevTime = now;

	auto&& rewardGenerator = scene.domain().global<slots::RewardGenerator>();
	auto slotMachineEntity = scene.domain().view<SlotMachine>().front();
	auto&& slotMachine = scene.domain().components<SlotMachine>().front();
	for (auto&& [entity, transform, slotObject] : scene.domain().view<scene::components::TransformComponent, SlotObject>().all()) {
		float adjSpeed = slotObject.speed * deltaTime;
		float adjAcc = slotObject.acceleration * deltaTime;
		float adjJolt = slotObject.jolt * deltaTime;

		// update physics
		transform.position.y += adjSpeed;
		slotObject.speed += adjAcc;
		slotObject.acceleration += adjJolt;

		//Logger::debug("{}", slotObject.speed);

		// STOP if hits min speed
		if (slotObject.jolt > 0 && slotObject.speed >= slotObject.minSpeed) {
			slotObject.speed = 0;
			slotObject.acceleration = 0;
			slotObject.jolt = 0;
			//Logger::debug("STOP");
		}

		// switch to slowdown
		if (slotObject.jolt < 0 && slotObject.speed <= slotObject.maxSpeed) {
			slotObject.jolt = -slotObject.jolt;
			//Logger::debug("jolt switch {}", slotObject.jolt);
		}

		// update pos if out of bounds
		if (transform.position.y < slotMachine.lowerBound) {
			auto diff = transform.position.y - slotMachine.lowerBound;
			transform.position.y = slotMachine.upperBound + diff;
			//Logger::debug("move slot to {1} {0}", slotMachine.upperBound, transform.position.y);

			scene.domain().getComponent<scene::components::MeshComponent>(entity).pipeline = slotMachine.symbols[(int)rewardGenerator.generateReward()];
		}
	}

	if (slotMachine.leverAnimationSpeed != 0) {
		auto&& [leverTransform] = scene.domain().view<scene::components::TransformComponent, LeverFlag>().components().front();
		auto&& [pawTransform, pawComp] = scene.domain().view<scene::components::TransformComponent, Paw>().components().front();
		auto adjLAS = slotMachine.leverAnimationSpeed * deltaTime;
		slotMachine.leverAnimation += adjLAS;

		if (slotMachine.leverAnimation >= 1) {
			slotMachine.leverAnimationSpeed = -slotMachine.leverAnimationSpeed;
			slotMachine.leverAnimation = 1;
		} else if (slotMachine.leverAnimation < 0) {
			slotMachine.leverAnimationSpeed = 0;
			slotMachine.leverAnimation = 0;
		}

		auto angle = -std::lerp(0.f, std::numbers::pi_v<float> / 2.f, slotMachine.leverAnimation);
		//Logger::debug("angle {}", angle);
		//Logger::debug("anim {}", slotMachine.leverAnimation);
		leverTransform.rotation = glm::angleAxis(angle, zAxis()) * glm::quat(0, 0, 0, 1);
		pawTransform.position.y = pawComp.originalY - slotMachine.leverAnimation * pawComp.movementScale;
	}
}