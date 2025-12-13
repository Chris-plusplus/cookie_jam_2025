#include <demon/demon1.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <Defaults.h>

void demon1::setup(Scene& scene) {
    auto head = scene.newEntity();
    auto headTexture = makeTexture("textures/Asset_szkice/Demon1_glowa_asset.png");
    auto&& renderer = *gfx::Renderer::current();
    head.addComponent<Demon1Flag>();
    auto&& transform = head.addComponent(
        scene::components::TransformComponent{
            .position = {windowWidth /4, windowHeight*0.3, 1},
            .rotation = {0, 0, 0, 1},
            .scale = {headTexture->getWidth()*0.8, headTexture->getHeight()*0.8, 0}
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
void demon1::show(Scene& scene) {
    auto head = scene.entitiesWith<Demon1Flag>().front();
    auto && transform = head.getComponent<scene::components::TransformComponent>();
    transform.position.z=-0.4;

}
void demon1::hide(Scene& scene) {
    auto head = scene.entitiesWith<Demon1Flag>().front();
    auto && transform = head.getComponent<scene::components::TransformComponent>();
    transform.position.z=1;
}