#include <lifes/LifeManagerSystem.h>
#include <lifes/LifeManager.h>
#include <Defaults.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <fstream>
#include <archimedes/Input.h>


void LifeManagerSystem::setup(Scene& scene) {
    auto manager = scene.newEntity();
    auto&& lifeManager = manager.addComponent<LifeManager>();
    auto&& renderer = *gfx::Renderer::current();

    // init lifes container sprite
    float2 slotTexSize{};
    auto&& containerTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_bar_asset.png"));
    slotTexSize.x = containerTexture->getWidth();
    slotTexSize.y = containerTexture->getHeight();
    auto&& transform = manager.addComponent(
        scene::components::TransformComponent{
            .position = {containerTexture->getWidth() / 2, windowHeight - containerTexture->getHeight() / 2, -0.5},
            .rotation = {0, 0, 0, 1},
            .scale = {containerTexture->getWidth() * 0.8, containerTexture->getHeight() * 0.8, 0}
        }
    );

    lifeManager.containerPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(containerTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );
    auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
    auto&& meshComp = manager.addComponent(
        scene::components::MeshComponent{
            .mesh = mesh,
            .pipeline = lifeManager.containerPipeline
        }
    );
    auto&& emptyCoinTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_token_nieaktywny.png"));
    slotTexSize.x = emptyCoinTexture->getWidth();
    slotTexSize.y = emptyCoinTexture->getHeight();
    lifeManager.emptyCoinPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(emptyCoinTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );

    auto&& fullCoinTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_token_aktywny.png"));
    slotTexSize.x = fullCoinTexture->getWidth();
    slotTexSize.y = fullCoinTexture->getHeight();
    lifeManager.fullCoinPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(fullCoinTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );
}

void LifeManagerSystem::update(Scene& scene) {}

void LifeManagerSystem::updateAnimation(Scene &scene) {}
