#include <demon/demon3.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <Defaults.h>

struct Demon3BodyFlag {
	static constexpr bool flagComponent = true;
};

void demon3::setup(Scene& scene) {
	{
		auto head = scene.newEntity();
		auto headTexture = makeTexture("textures/demon3.png");
		auto&& renderer = *gfx::Renderer::current();
		head.addComponent<Demon3Flag>();
		auto&& transform = head.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, windowHeight / 2, 1},
				.rotation = {0, 0, 0, 1},
				.scale = float3{headTexture->getWidth(), headTexture->getHeight(), 0} *0.9f
			}
		);
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = renderer.getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {std::move(headTexture)},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& meshComp = head.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline
			}
		);
	}
	{
		auto head = scene.newEntity();
		auto headTexture = makeTexture("textures/demon3_2.png");
		auto&& renderer = *gfx::Renderer::current();
		head.addComponent<Demon3BodyFlag>();
		auto&& transform = head.addComponent(
			scene::components::TransformComponent{
				.position = {windowWidth / 2, windowHeight / 2, 1},
				.rotation = {0, 0, 0, 1},
				.scale = float3{headTexture->getWidth(), headTexture->getHeight(), 0} *0.9f
			}
		);
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto pipeline = renderer.getPipelineManager()->create(
			gfx::pipeline::Pipeline::Desc{
				.vertexShaderPath = "shaders/vertex_default.glsl",
				.fragmentShaderPath = "shaders/fragment_default.glsl",
				.textures = {std::move(headTexture)},
				.buffers = {defaultUniformBuffer()},
			}
			);
		auto&& meshComp = head.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = pipeline
			}
		);
	}
}
void demon3::show(Scene& scene) {
	{
		auto head = scene.entitiesWith<Demon3Flag>().front();
		auto&& transform = head.getComponent<scene::components::TransformComponent>();
		transform.position.z = -0.4;
	}
	{
		auto head = scene.entitiesWith<Demon3BodyFlag>().front();
		auto&& transform = head.getComponent<scene::components::TransformComponent>();
		transform.position.z = -0.505;
	}
}
void demon3::hide(Scene& scene) {
	{
		auto head = scene.entitiesWith<Demon3Flag>().front();
		auto&& transform = head.getComponent<scene::components::TransformComponent>();
		transform.position.z = 1;
	}
	{
		auto head = scene.entitiesWith<Demon3BodyFlag>().front();
		auto&& transform = head.getComponent<scene::components::TransformComponent>();
		transform.position.z = 1;
	}
}