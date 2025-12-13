#include <demon/demon1.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <Defaults.h>

void demon1::setup(Scene& scene) {
    auto head = scene.newEntity();
    auto headTexture = makeTexture("textures/Asset_szkice/Demon1_glowa_asset.png");
    auto&& renderer = *gfx::Renderer::current();
    auto&& transform = head.addComponent(
        scene::components::TransformComponent{
            .position = {windowWidth /4, windowHeight*0.3, -0.4},
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