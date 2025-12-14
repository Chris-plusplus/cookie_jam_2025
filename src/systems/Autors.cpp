#include <systems/Autors.h>
#include <components/Pledge.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <systems/AutorsFlag.h>

#include "button/Button.h"

void Autors::setup(Scene& scene) {
    if (scene.domain().components<Autors>().base().count() != 0) {
        return;
    }
    {
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
    {
        auto Exit = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Zamyknie_asset.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = Exit.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.6, windowHeight * 0.85, -0.9},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth(), texture->getHeight(), 0}
            }
        );
        Exit.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        Exit.addComponent(
            Button{
                .topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
                .bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
                .callback = [&](...) { exit(scene); }
            });
        auto&& flag = Exit.addComponent(AutorsFlag{});
    }
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
