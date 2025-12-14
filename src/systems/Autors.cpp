#include <systems/Autors.h>
#include <components/Pledge.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <systems/AutorsFlag.h>

void Autors::setup(Scene& scene) {
    if (scene.domain().components<Autors>().base().count() != 0) {
        return;
    }
    auto Autors = scene.newEntity();
    auto&& containerTexture = makeTexture(std::string_view("textures/Asset_final/Tworcy_fin 2.png"));
    auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
    auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
        gfx::pipeline::Pipeline::Desc{
            .vertexShaderPath = "shaders/vertex_default.glsl",
            .fragmentShaderPath = "shaders/fragment_default.glsl",
            .textures = {containerTexture},
            .buffers = {defaultUniformBuffer()},
        }
        );
    auto&& transform = Autors.addComponent(
        scene::components::TransformComponent{
            .position = {windowWidth / 2, windowHeight / 2, -0.1},
            .rotation = {0, 0, 0, 1},
            .scale = {containerTexture->getWidth()*0.8, containerTexture->getHeight()*0.8, -0.9}
        }
    );
    auto&& meshComp = Autors.addComponent(
        scene::components::MeshComponent{
            .mesh = mesh,
            .pipeline = pipeline,
        }
    );
    auto&& flag = Autors.addComponent(AutorsFlag{});
}

void Autors::exit(Scene& scene) {
    std::vector<ecs::Entity> toKill;
    for (auto&& entity : scene.domain().view<AutorsFlag>()) {
        toKill.push_back(entity);
    }
    for (auto&& entity : toKill) {
        scene.domain().kill(entity);
    }
}
