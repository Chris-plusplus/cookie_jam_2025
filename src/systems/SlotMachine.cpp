#include <components/SlotMachine.h>
#include <systems/SlotMachine.h>
#include <Defaults.h>
#include <slots/SlotsManager.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <fstream>
#include <slots/SlotFlag.h>
#include <slots/SlotObject.h>
#include <archimedes/Input.h>

void SlotMachineSystem::setup(Scene& scene) {
	auto machine = scene.newEntity();
	auto&& slotMachine = machine.addComponent<SlotMachine>();
	auto&& renderer = *gfx::Renderer::current();

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
			.scale = {machineTexture->getWidth(), machineTexture->getHeight(), 0}
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
}

void SlotMachineSystem::update(Scene& scene) {
	if (input::Keyboard::space.pressed()) {
		for (auto&& [slotObject] : scene.domain().view<SlotObject>().components()) {
			if (slotObject.jolt == 0) {
				slotObject.jolt = -1.f;
			}
		}
	}
	updateAnimation(scene);
}

void SlotMachineSystem::updateAnimation(Scene& scene) {
	auto&& slotMachine = scene.domain().components<SlotMachine>().front();
	for (auto&& [transform, slotObject] : scene.domain().view<scene::components::TransformComponent, SlotObject>().components()) {
		// update physics
		transform.position.y += slotObject.speed;
		slotObject.speed += slotObject.acceleration;
		slotObject.acceleration += slotObject.jolt;

		// STOP if hits min speed
		if (slotObject.jolt > 0 && slotObject.speed >= slotObject.minSpeed) {
			slotObject.speed = 0;
			slotObject.acceleration = 0;
			slotObject.jolt = 0;
			Logger::debug("STOP");
		}

		// switch to slowdown
		if (slotObject.jolt < 0 && slotObject.speed <= slotObject.maxSpeed) {
			slotObject.jolt = -slotObject.jolt;
			Logger::debug("jolt switch {}", slotObject.jolt);
		}

		// update pos if out of bounds
		if (transform.position.y < slotMachine.lowerBound) {
			auto diff = transform.position.y - slotMachine.lowerBound;
			transform.position.y = slotMachine.upperBound + diff;
			Logger::debug("move slot to {1} {0}", slotMachine.upperBound, transform.position.y);
		}
	}
}