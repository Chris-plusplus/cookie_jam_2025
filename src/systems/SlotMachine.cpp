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
#include <lifes/LifeManager.h>
#include <cmath>
#include <numbers>
#include <demon/DemonManager.h>
#include <demon/PositiveSwitch.h>
#include <demon/NegativeSwitch.h>
#include <demon/SwitchSystem.h>
#include <slots/SlotGlitchChance.h>
#include <systems/EndingSystem.h>

#include "sound/SFX.h"

using namespace std::chrono_literals;

struct LeverFlag {
	static constexpr bool flagComponent = true;
};

struct Paw {
	float originalY;
	float movementScale;
	std::chrono::milliseconds waitTime;
	decltype(std::chrono::high_resolution_clock::now()) prevTime;
	bool waiting = false;
};

namespace {
auto rng = std::mt19937(std::random_device{}());
}
bool glitched = false;

bool SlotMachineSystem::isEnd = false;

void SlotMachineSystem::setup(Scene& scene) {
	auto machine = scene.newEntity();
	auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
	auto&& slotMachine = machine.addComponent<SlotMachine>();
	auto&& renderer = *gfx::Renderer::current();

	scene.domain().global<slots::RewardGenerator>();

	float2 slotTexSize{};
	// init slots' pipelines
	for (auto i = 0; i != 3; ++i) {
		auto&& glitchedPipeline = slotMachine.glitchedSymbols.emplace_back();
		auto&& texture = makeTexture(std::string_view(std::format("textures/slots/glitch{}.png", i)));
		glitchedPipeline = renderer.getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {std::move(texture)},
				.buffers = {defaultUniformBuffer()},
			}
			);
	}
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

	{ // init machine sprite
		auto machineTexture = makeTexture("textures/slotMachine.png");
		auto&& transform = machine.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, windowHeight / 2, -0.5},
				.rotation = {0, 0, 0, 1},
				.scale = float3{machineTexture->getWidth(), machineTexture->getHeight(), 0} *0.9f
			}
		);
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
	}
	{ // init background sprite
		auto background = scene.newEntity();
		auto backgroundTexture = makeTexture("textures/background.png");
		auto&& transform = background.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, windowHeight / 2, -0.3},
				.rotation = {0, 0, 0, 1},
				.scale = float3{backgroundTexture->getWidth(), backgroundTexture->getHeight(), 0} *0.9f
			}
		);
		auto pipeline = renderer.getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {std::move(backgroundTexture)},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& meshComp = background.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline
			}
		);
	}
	{ // init cat sprite
		auto cat = scene.newEntity();
		auto catTexture = makeTexture("textures/catHead.png");
		auto&& transform = cat.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, catTexture->getHeight() / 2, -0.51},
				.rotation = {0, 0, 0, 1},
				.scale = float3{catTexture->getWidth(), catTexture->getHeight(), 0}
			}
		);
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = renderer.getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {std::move(catTexture)},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& meshComp = cat.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline
			}
		);
	}

	{ // init slots objects
		auto file = std::ifstream("slotConfig3.txt");
		for (auto i = 0; i != slotMachine.slotCount; ++i) {
			float originalX{}, originalY{};
			file >> originalX >> originalY;

			slotMachine.lowerBound = originalY - 1.5f * slotTexSize.y;
			slotMachine.upperBound = originalY + 1.5f * slotTexSize.y;

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
						.pipeline = slotMachine.symbols[(int)scene.domain().global<slots::RewardGenerator>().generateReward()]
					}
				);
				slotEntity.addComponent<SlotObject>();
			}
		}
	}

	{ // init lever
		auto lever = scene.newEntity();
		lever.addComponent<LeverFlag>();
		float3 leverPos{0, 0, -0.475};
		{
			auto file = std::ifstream("leverPos3.txt");
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
			auto file = std::ifstream("pawPos3.txt");
			auto&& pawComp = paw.addComponent<Paw>(0, 0, std::chrono::milliseconds(300), std::chrono::high_resolution_clock::now());
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
	auto&& manager = scene.domain().view<LifeManager>().front();
	auto&& lifeManager = scene.domain().getComponent<LifeManager>(manager);
	if ((input::Keyboard::space.pressed() || glitched)
		&& !slotMachine.slotAnimation &&
		(lifeManager.currentLifes > 0 || glitched)
		&& DemonManager::active_demon == demon::DemonType::_none) {
		if (!glitched) {
			lifeManager.updateLifes(-1);
			slotMachine.leverAnimationSpeed = 10;
			slotMachine.pawAnimationSpeed = 10;
			SFX::playSFX("wajcha.ogg");
		}
		slotMachine.slotAnimation = true;
		slotMachine.drawn.clear();
		DemonManager::isBlocked = true;

		for (auto&& [slotObject] : scene.domain().view<SlotObject>().components()) {
			if (slotObject.jolt == 0) {
				slotObject.jolt = -10000.f;
			}
		}
	}
	updateAnimation(scene);
}

struct SlotStride {
	float value;
};

void SlotMachineSystem::updateAnimation(Scene& scene) {
	if (isEnd) {
		return;
	}
	static auto prevTime = std::chrono::high_resolution_clock::now();
	const auto now = std::chrono::high_resolution_clock::now();
	auto deltaTime = (float)std::chrono::duration_cast<decltype(0.0s)>(now - prevTime).count();
	prevTime = now;

	auto&& rewardGenerator = scene.domain().global<slots::RewardGenerator>();
	auto slotMachineEntity = scene.domain().view<SlotMachine>().front();
	auto&& slotMachine = scene.domain().components<SlotMachine>().front();
	bool resetStride = false;
	for (auto&& [entity, transform, slotObject] : scene.domain().view<scene::components::TransformComponent, SlotObject>().all()) {
		float adjSpeed = slotObject.speed * deltaTime;
		float adjAcc = slotObject.acceleration * deltaTime;
		float adjJolt = slotObject.jolt * deltaTime;

		if (adjSpeed > 0) {
			Logger::debug("{}", adjSpeed);
		}

		// update physics
		transform.position.y += adjSpeed;
		slotObject.speed += adjAcc;
		slotObject.acceleration += adjJolt;

		//Logger::debug("{}", slotObject.speed);

		if (slotObject.stride) {
			slotObject.stride += adjSpeed;
			if (slotObject.stride < 0) {
				slotObject.speed = 0;
				slotObject.stride = 0;
				resetStride = true;
				transform.position.y -= slotObject.stride;
			}
		}

		// STOP if hits min speed
		if (slotObject.jolt > 0 && slotObject.speed >= slotObject.minSpeed) {
			slotObject.acceleration = 0;
			slotObject.jolt = 0;

			slotMachine.strideCompute = true;
			slotMachine.stride = true;
			slotObject.speed = slotObject.minSpeed;

			//slotObject.speed = 0;
			//slotMachine.slotStride = false;
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

			slotObject.type = rewardGenerator.generateReward();
			if (!glitched) {
				scene.domain().getComponent<scene::components::MeshComponent>(entity).pipeline = slotMachine.symbols[(int)slotObject.type];
			}
		}
	}

	// animation final end
	if (resetStride) {
		// flags
		resetStride = false;
		slotMachine.stride = false;
		slotMachine.slotAnimation = false;

		// check reward
		auto center = (slotMachine.upperBound + slotMachine.lowerBound) / 2;
		std::vector<SlotObject*> drawnObj;
		std::vector<ecs::Entity> drawnObjEnt;
		for (auto&& col : slotMachine.slotsByCollumn) {
			// find closest to center in collumn
			float min = std::numeric_limits<float>::infinity();
			SlotObject* minObj = nullptr;
			ecs::Entity minEnt = ecs::nullEntity;
			for (auto&& slotEntity : col) {
				auto&& slotObject = scene.domain().getComponent<SlotObject>(slotEntity);
				auto&& transform = scene.domain().getComponent<scene::components::TransformComponent>(slotEntity);
				auto diff = std::fabs(transform.position.y - center);
				if (diff < min) {
					min = diff;
					minObj = &slotObject;
					minEnt = slotEntity;
				}
			}
			Logger::debug("got = {}", slots::rewardAsString(minObj->type));
			slotMachine.drawn.push_back((int)minObj->type);
			drawnObj.push_back(minObj);
			drawnObjEnt.push_back(minEnt);
		}

		auto switchVal = SwitchSystem::getSwitch(scene);
		auto getScore = [&slotMachine] {
			std::array<int, (int)slots::RewardType::_count> counts{};
			for (auto&& v : slotMachine.drawn) {
				++counts[v];
			}
			int count = 0, val = 0;
			for (int i = 0; i != counts.size(); ++i) {
				if (counts[i] > count) {
					count = counts[i];
					val = i;
				}
			}
			return std::tuple{count, val};
		};

		//Logger::debug("switchVal = {}", switchVal);
		auto&& [count, what] = getScore();
		//Logger::debug("count = {}", count);
		//Logger::debug("what = {}", slots::rewardAsString((slots::RewardType)what));
		if (switchVal < 0 && count == slotMachine.drawn.size()) {
			auto i = SwitchSystem::indexToSwitch(slotMachine.drawn.size() - 1);
			auto original = drawnObj[i]->type;
			while (drawnObj[i]->type == original) {
				drawnObj[i]->type = scene.domain().global<slots::RewardGenerator>().generateReward();
				slotMachine.drawn[i] = (int)drawnObj[i]->type;
			}
			scene.domain().getComponent<scene::components::MeshComponent>(drawnObjEnt[i]).pipeline = slotMachine.symbols[(int)drawnObj[i]->type];
		} else if (switchVal > 0 && count == slotMachine.drawn.size() - 1) {
			auto i = std::ranges::find_if_not(slotMachine.drawn, [what](auto x) { return x == what; }) - slotMachine.drawn.begin();
			drawnObj[i]->type = (slots::RewardType)what;
			slotMachine.drawn[i] = what;
			scene.domain().getComponent<scene::components::MeshComponent>(drawnObjEnt[i]).pipeline = slotMachine.symbols[(int)drawnObj[i]->type];
		}

		if (glitched) {
			glitched = false;
			for (auto&& [slotObj, mesh] : scene.domain().view<SlotObject, scene::components::MeshComponent>().components()) {
				slotObj.glitched = false;
				mesh.pipeline = slotMachine.symbols[(int)slotObj.type];
			}
		}

		// glitch
		auto glitchChance = scene.domain().global<SlotGlitchChance>().value;
		if (!glitched && std::uniform_real_distribution(0.f, 1.f)(rng) <= glitchChance) {
			glitched = true;
			for (auto&& [slotObj, mesh] : scene.domain().view<SlotObject, scene::components::MeshComponent>().components()) {
				slotObj.glitched = true;
				mesh.pipeline = slotMachine.glitchedSymbols[std::uniform_int_distribution(0, 2)(rng)];
			}
		} else {
			if (count == slotMachine.drawn.size() && what == (int)slots::RewardType::cucumber) {
				isEnd = true;
				EndingSystem::end(scene, "textures/Asset_final/Ogor_ending.png", "bad_ending_theme.ogg");
			} else {
				if (slotMachine.onDrawn) {
					slotMachine.onDrawn(scene, slotMachine.drawn);
				}
				DemonManager::addroll();
				DemonManager::isBlocked = false;
				Logger::debug("reward = {}", slots::rewardAsString(SlotMachineSystem::reward(scene)));


				if (scene.domain().components<LifeManager>().front().currentLifes == 0) {

					DemonManager::isBlocked = true;
					EndingSystem::end(scene, "textures/Asset_final/Bad_ending.png", "bad_ending_theme.ogg");
					isEnd = true;
				}
			}
		}
	}

	if (slotMachine.strideCompute) {
		slotMachine.strideCompute = false;
		float min = std::numeric_limits<float>::infinity();
		for (auto&& [entity, transform, slotObject] : scene.domain().view<scene::components::TransformComponent, SlotObject>().all()) {
			auto diff = transform.position.y - (slotMachine.lowerBound + slotMachine.upperBound) / 2;
			if (diff >= 0 && diff < min) {
				min = diff;
			}
		}
		for (auto&& [slotObject] : scene.domain().view<SlotObject>().components()) {
			slotObject.stride = min;
		}
	}

	if (slotMachine.leverAnimationSpeed != 0) {
		// lever
		auto&& [leverTransform] = scene.domain().view<scene::components::TransformComponent, LeverFlag>().components().front();
		auto adjLAS = slotMachine.leverAnimationSpeed * deltaTime;
		slotMachine.leverAnimation += adjLAS;

		if (slotMachine.leverAnimation >= 1) {
			slotMachine.leverAnimationSpeed = -5;
			slotMachine.leverAnimation = 1;
		} else if (slotMachine.leverAnimation < 0) {
			slotMachine.leverAnimationSpeed = 0;
			slotMachine.leverAnimation = 0;
		}

		auto angle = -std::lerp(0.f, std::numbers::pi_v<float> / 2.f, slotMachine.leverAnimation);
		//Logger::debug("angle {}", angle);
		//Logger::debug("anim {}", slotMachine.leverAnimation);
		leverTransform.rotation = glm::angleAxis(angle, zAxis()) * glm::quat(0, 0, 0, 1);
	}

	auto&& [pawTransform, pawComp] = scene.domain().view<scene::components::TransformComponent, Paw>().components().front();
	if (slotMachine.pawAnimationSpeed != 0) {
		// paw
		auto adjPAS = slotMachine.pawAnimationSpeed * deltaTime;
		slotMachine.pawAnimation += adjPAS;

		if (slotMachine.pawAnimation >= 1) {
			slotMachine.pawAnimationSpeed = 0;
			slotMachine.pawAnimation = 1;

			pawComp.prevTime = std::chrono::high_resolution_clock::now();
			pawComp.waiting = true;
		} else if (slotMachine.pawAnimation < 0) {
			slotMachine.pawAnimationSpeed = 0;
			slotMachine.pawAnimation = 0;
		}
		pawTransform.position.y = pawComp.originalY - slotMachine.pawAnimation * pawComp.movementScale;
	}
	if (pawComp.waiting && now - pawComp.prevTime > pawComp.waitTime) {
		slotMachine.pawAnimationSpeed = -5;
		pawComp.waiting = false;
	}
}

slots::RewardType SlotMachineSystem::reward(Scene& scene) {
	auto&& drawn = scene.domain().components<SlotMachine>().front().drawn;
	return drawn.size() == 0 ? slots::RewardType::_none : (std::ranges::count(drawn, drawn.front()) == drawn.size() ? (slots::RewardType)drawn.front() : slots::RewardType::_none);
}

const std::vector<int>& SlotMachineSystem::drawn(Scene& scene) {
	return scene.domain().components<SlotMachine>().front().drawn;
}

void SlotMachineSystem::onDrawn(Scene& scene, std::function<int(Scene&, const std::vector<int>&)> event) {
	scene.domain().components<SlotMachine>().front().onDrawn = event;
}