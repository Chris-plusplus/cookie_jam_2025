#include <systems/TakenDeals.h>
#include <components/Pledge.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <systems/TakenDealsFlag.h>

#include "button/Button.h"

void TakenDeals::setup(Scene& scene) {
    {
        auto AccesCard = scene.newEntity();
        auto&& containerTexture = makeTexture(std::string_view("textures/Asset_final/Button_efekty.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {containerTexture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = AccesCard.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth*0.94, windowHeight*0.86, -0.6},
                .rotation = {0, 0, 0, 1},
                .scale = {containerTexture->getWidth(), containerTexture->getHeight(), 0}
            }
        );
        auto&& meshComp = AccesCard.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        AccesCard.addComponent(
            Button{
                .topLeft = float2{-t.scale.x, t.scale.y} / 2.f,
                .bottomRight = float2{t.scale.x, -t.scale.y} / 2.f,
                .callback = [&](...) { show(scene); }
            });

    }
}

void TakenDeals::show(Scene& scene) {
    {
        auto Page = scene.newEntity();
        auto&& containerTexture = makeTexture(std::string_view("textures/Asset_final/Pakty_karta.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {containerTexture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& transform = Page.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth / 2, windowHeight / 2, -0.91},
                .rotation = {0, 0, 0, 1},
                .scale = {containerTexture->getWidth(), containerTexture->getHeight(), 0}
            }
        );
        auto&& meshComp = Page.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = Page.addComponent(TakenDealsFlag{});
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
                .position = {windowWidth * 0.6, windowHeight * 0.85, -0.92},
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
        auto&& flag = Exit.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_chillowy_1.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.73, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_chillowy_2.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.64, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_chillowy_3.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.55, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_zmulony_1.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.46, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_zmulony_2.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.37, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_zmulony_3.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.28, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_rozdwojenie_1.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.19, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_rozdwojenie_2.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.10, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
    {
        auto pakt = scene.newEntity();
        auto&& texture = makeTexture(std::string_view("textures/Asset_final/Pakt_rozdwojenie_3.png"));
        auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
        auto pipeline = gfx::Renderer::getCurrent()->getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {texture},
                .buffers = {defaultUniformBuffer()},
            }
            );
        auto&& t = pakt.addComponent(
            scene::components::TransformComponent{
                .position = {windowWidth * 0.5, windowHeight * 0.2, -0.92},
                .rotation = {0, 0, 0, 1},
                .scale = {texture->getWidth()/2, texture->getHeight()/2, 0}
            }
        );
        pakt.addComponent(
            scene::components::MeshComponent{
                .mesh = mesh,
                .pipeline = pipeline,
            }
        );
        auto&& flag = pakt.addComponent(TakenDealsFlag{});
    }
}

void TakenDeals::exit(Scene& scene) {
    std::vector<ecs::Entity> toKill;
    for (auto&& entity : scene.domain().view<TakenDealsFlag>()) {
        toKill.push_back(entity);
    }
    for (auto&& entity : toKill) {
        scene.domain().kill(entity);
    }
}
