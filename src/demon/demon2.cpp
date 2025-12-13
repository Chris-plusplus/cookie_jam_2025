#include <demon/demon2.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <Defaults.h>

void demon2::setup(Scene& scene) {
    auto head = scene.newEntity();
    auto headTexture = makeTexture("textures/demon_nr2_dyzio.png");
    auto&& renderer = *gfx::Renderer::current();
    head.addComponent<Demon2Flag>();
    auto&& transform = head.addComponent(
        scene::components::TransformComponent{
            .position = {windowWidth *0.7, windowHeight*0.43, 1},
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
void demon2::show(Scene& scene) {
    auto head = scene.entitiesWith<Demon2Flag>().front();
    auto && transform = head.getComponent<scene::components::TransformComponent>();
    transform.position.z=-0.7;

}
void demon2::hide(Scene& scene) {
    auto head = scene.entitiesWith<Demon2Flag>().front();
    auto && transform = head.getComponent<scene::components::TransformComponent>();
    transform.position.z=1;
}