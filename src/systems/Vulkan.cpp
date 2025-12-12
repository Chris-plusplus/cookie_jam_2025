#include <components/Vulkan.h>
#include <systems/Vulkan.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <systems/Explosion.h>
#include <archimedes/physics/components/Moveable.h>
#include <archimedes/audio/AudioSourceComponent.h>
#include <SoundManager.h>


void setupAudioSource(Entity vulkan, const Vulkan& vulkanComp);

void VulkanSystem::setup(Entity vulkan, input::Key& key, bool isRight) {
	auto renderer = gfx::Renderer::getCurrent();

	// load textures
	auto vulkanTexture = makeTexture("textures/vulkan.png");
	auto particleTexture = makeTexture("textures/particle.png");

	const auto texWidth = vulkanTexture->getWidth();
	const auto texHeight = vulkanTexture->getHeight();

	// make mesh and pipelines
	auto mesh = makeMesh(std::span(defaultVertices()), std::span(defaultIndices()));
	auto pipeline = renderer->getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {vulkanTexture},
			.buffers = {defaultUniformBuffer()},
		});
	auto particlePipeline = renderer->getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {particleTexture},
			.buffers = {defaultUniformBuffer()},
		});

	const float scale = 0.25f;
	const float windowLeftSpace = (windowWidth - 2.f * scale * texWidth) / 3.f;
	auto&& transform = vulkan.addComponent(
		scene::components::TransformComponent{
			.position = float3{windowLeftSpace + isRight * (windowLeftSpace + texWidth * scale), texHeight * scale, 0},
			.rotation = {0, 0, 0, 1},
			.scale = scale * float3{texWidth, texHeight, 0},
		});
	vulkan.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh,
			.pipeline = pipeline
		});

	using FloatDist = Vulkan::FloatDist;
	auto&& vulkanComponent = vulkan.addComponent(
		Vulkan{
			// volcano settings
			.particleCount = 200,
			.key = &key,
			.explosionAngleDist = FloatDist(glm::radians(-45.f), glm::radians(45.f)),
			.velocityDist = FloatDist(600.f, 1000.f),
			.rotationSpeedDist = FloatDist(glm::radians(-9.f), glm::radians(9.f)),
			.sizeDist = FloatDist(20.f, 40.f),
			.eDist = FloatDist(0.25f, 0.4f), // bounciness
			.texture = vulkanTexture,
			.particlePipeline = particlePipeline,
			.particleMesh = makeMesh(std::span(defaultCenterVertices()), std::span(defaultIndices())),
			.particleOrigin = transform.position + float3{texWidth * scale / 2.f, 0, 0},
			.isRight = isRight
		}
	);

	setupAudioSource(vulkan, vulkanComponent);
}

void setupAudioSource(Entity vulkan, const Vulkan& vulkanComp) {
	auto&& sourceComp = vulkan.addComponent<audio::AudioSourceComponent>();
	// audio settings
	sourceComp.path = explosionSoundPath;
	sourceComp.isLooped = false;
	sourceComp.rolloffFactor = 0.01f;
	sourceComp.dontRemoveFinished = true;

	auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
	soundManager.audioManager->assignSource(sourceComp);
}

void VulkanSystem::update(Scene& scene) {
	for (auto&& [entity, vulkan] : scene.domain().view<Vulkan>().all()) {
		if (vulkan.key->pressed()) {
			ExplosionSystem::makeExplosion(scene, Entity(scene, entity));
		}
	}
}

