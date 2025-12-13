#include <demon/OfferSystem.h>
#include <Defaults.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <fstream>
#include <archimedes/Input.h>
#include "MakeTexture.h"
#include "demon/OfferDialogue.h"
#include "demon/AcceptButtonFlag.h"
#include "demon/DismissButtonFlag.h"

void OfferSystem::setup(Scene& scene) {
    auto system = scene.newEntity();
    auto&& offerDialogue = system.addComponent<demon::OfferDialogue>();
    auto&& renderer = *gfx::Renderer::current();

    float2 slotTexSize{};
    //TODO replace textures
    auto&& containerTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_bar_asset.png"));
    slotTexSize.x = containerTexture->getWidth();
    slotTexSize.y = containerTexture->getHeight();
    offerDialogue.buttonWidth = containerTexture->getWidth();
    offerDialogue.buttonHeight = containerTexture->getHeight();
    offerDialogue.buttonPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(containerTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );
    offerDialogue.buttonPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(containerTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );


    offerDialogue.acceptButtonX = offerDialogue.buttonWidth / 2;
    offerDialogue.acceptButtonY = windowHeight - offerDialogue.buttonHeight / 2;
    offerDialogue.buttonScaleX = 0.8;
    offerDialogue.buttonScaleY = 0.8;

    auto&& transform = system.addComponent(
        scene::components::TransformComponent{
            .position = {offerDialogue.acceptButtonX, offerDialogue.acceptButtonY, -0.5},
            .rotation = {0, 0, 0, 1},
            .scale = {offerDialogue.buttonWidth * offerDialogue.buttonScaleX, offerDialogue.buttonHeight * offerDialogue.buttonScaleY, 0}
        }
    );
    auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
    auto&& meshComp = system.addComponent(
        scene::components::MeshComponent{
            .mesh = mesh,
            .pipeline = offerDialogue.buttonPipeline
        }
    );
    auto&& buttonFlag = system.addComponent(AcceptButtonFlag{});

    offerDialogue.dismissButtonX = offerDialogue.buttonWidth;
    offerDialogue.dismissButtonY = windowHeight - offerDialogue.buttonHeight;

    auto&& transform = system.addComponent(
        scene::components::TransformComponent{
            .position = {offerDialogue.dismissButtonX, offerDialogue.dismissButtonY, -0.5},
            .rotation = {0, 0, 0, 1},
            .scale = {offerDialogue.buttonWidth * offerDialogue.buttonScaleX, offerDialogue.buttonHeight * offerDialogue.buttonScaleY, 0}
        }
    );
    auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
    auto&& meshComp = system.addComponent(
        scene::components::MeshComponent{
            .mesh = mesh,
            .pipeline = offerDialogue.dismissButtonPipeline
        }
    );
    auto&& buttonFlag = system.addComponent(DismissButtonFlag{});


}
