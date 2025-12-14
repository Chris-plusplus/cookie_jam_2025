#include <systems/PledgeSystem.h>
#include <components/Pledge.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>

#include "sound/SFX.h"

auto prevTime = std::chrono::high_resolution_clock::now();

void PledgeSystem::setup(Scene& scene) {
	if (scene.domain().components<PledgeAnimation>().base().count() != 0) {
		return;
	}

	auto&& textures = scene.domain().global<PledgeTextures>();
	if (!textures.dark) {
		auto renderer = gfx::Renderer::getCurrent();
		Color color{};
		{
			auto file = std::ifstream("pledgeDarkColor.txt");
			file >> color.r >> color.g >> color.b >> color.a;
		}
		textures.dark = renderer->getTextureManager()->createTexture2D(1, 1, &color);
		textures.darkPipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {textures.dark},
				.buffers = {defaultUniformBuffer()}
			}
		);

		textures.paw = makeTexture("textures/paw.png");
		textures.pawPipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {textures.paw},
				.buffers = {defaultUniformBuffer()}
			}
		);

		textures.noPaw = makeTexture("textures/pledge.png");
		textures.noPawPipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {textures.noPaw},
				.buffers = {defaultUniformBuffer()}
			}
		);

		textures.withPaw = makeTexture("textures/pledgePaw.png");
		textures.withPawPipeline = renderer->getPipelineManager()->create(
			{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {textures.withPaw},
				.buffers = {defaultUniformBuffer()}
			}
		);
	}

	auto pledge = scene.newEntity();
	auto dark = scene.newEntity();
	dark.addComponent<PlegdeDarkFlag>();
	auto paw = scene.newEntity();
	paw.addComponent<PlegdePawFlag>();

	auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());

	pledge.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2, windowHeight / 2, -0.9},
			.rotation = {0, 0, 0, 1},
			.scale = float3{textures.noPaw->getWidth(), textures.noPaw->getHeight(), 0} / 1.2f
		}
	);
	pledge.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh,
			.pipeline = textures.noPawPipeline
		}
	);
	{
		float begin{};
		float end{};
		float speed{};
		{
			auto file = std::ifstream("pledgePawBeginEnd.txt");
			file >> begin >> end >> speed;
		}
		pledge.addComponent(
			PledgeAnimation{
				.waitTime = 0.1s,
				.animSpeed = speed,
				.beginPos = begin,
				.endPos = end
			}
		);
	}

	dark.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2, windowHeight / 2, -0.89},
			.rotation = {0, 0, 0, 1},
			.scale = {windowWidth, windowHeight, 0}
		}
	);
	dark.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh,
			.pipeline = textures.darkPipeline
		}
	);

	{
		float2 pawPos{};
		{
			auto file = std::ifstream("pledgePawPos.txt");
			file >> pawPos.x >> pawPos.y;
		}
		paw.addComponent(
			scene::components::TransformComponent{
				.position = {pawPos, -0.91},
				.rotation = {0, 0, 0, 1},
				.scale = float3{textures.paw->getWidth(), textures.paw->getHeight(), 0} / 2.f
			}
		);
		paw.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = textures.pawPipeline
			}
		);
	}

	prevTime = std::chrono::high_resolution_clock::now();
}

void PledgeSystem::update(Scene& scene) {
	if (scene.domain().components<PledgeAnimation>().base().count() != 0) {
		auto now = std::chrono::high_resolution_clock::now();
		auto deltaTimeDur = std::chrono::duration_cast<decltype(0.0s)>(now - prevTime);
		auto deltaTime = (float)deltaTimeDur.count();
		prevTime = now;

		auto&& anim = scene.domain().components<PledgeAnimation>().front();

		auto adjSpeed = anim.animSpeed * deltaTime;
		auto&& [pawT] = scene.domain().view<scene::components::TransformComponent, PlegdePawFlag>().components().front();
		if (anim.animSpeed < 0) {
			if (anim.waitTime > 0.0s) {
				anim.waitTime -= deltaTimeDur;
			} else {
				pawT.position.y += adjSpeed;
				if (pawT.position.y <= anim.beginPos) {
					pawT.position.y = anim.beginPos;
					anim.animSpeed = 0;
					auto&& callback = scene.domain().global<PledgeAnimationEndCallback>();
					if (callback.onAnimationEnd) {
						callback.onAnimationEnd();
					}
					PledgeSystem::cleanup(scene);
					return;
				}
			}
		} else {
			pawT.position.y += adjSpeed;

			if (pawT.position.y >= anim.endPos) {
				SFX::playSFX("miau.ogg");
				pawT.position.y = anim.endPos;
				anim.animSpeed = -anim.animSpeed;
				scene.domain().getComponent<scene::components::MeshComponent>(
					scene.domain().view<PledgeAnimation>().front()
				).pipeline = scene.domain().global<PledgeTextures>().withPawPipeline;
			}
		}
	}
}

void PledgeSystem::cleanup(Scene& scene) {
	std::vector<ecs::Entity> toKill;
	for (auto&& entity : scene.domain().view<PledgeAnimation>()) {
		toKill.push_back(entity);
	}
	for (auto&& entity : scene.domain().view<PlegdePawFlag>()) {
		toKill.push_back(entity);
	}
	for (auto&& entity : scene.domain().view<PlegdeDarkFlag>()) {
		toKill.push_back(entity);
	}
	for (auto&& entity : toKill) {
		scene.domain().kill(entity);
	}
}

void PledgeSystem::setCallback(Scene& scene, std::function<void()> callback) {
	scene.domain().global<PledgeAnimationEndCallback>().onAnimationEnd = callback;
}
