#include <demon/demon3.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <Defaults.h>

void demon3::setup(Scene& scene) {
    auto head = scene.newEntity();
    auto headTexture = makeTexture("textures/Asset_szkice/Demon3_glowa_asset.png");
    auto&& renderer = *gfx::Renderer::current();
    auto&& transform = head.addComponent(
        scene::components::TransformComponent{
            .position = {windowWidth /5, windowHeight*0.4, -0.4},
            .rotation = {0, 0, 0, 1},
            .scale = {headTexture->getWidth()*0.65, headTexture->getHeight()*0.7, 0}
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