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

namespace demon {
void OfferSystem::spawnOfferDialogue(Scene& scene) {
    auto&& manager = scene.domain().view<OfferDialogue>().front();
    auto&& offerDialogue = scene.domain().getComponent<OfferDialogue>(manager);

    auto&& renderer = *gfx::Renderer::current();

    auto&& acceptTexture = makeTexture(std::string_view("textures/Asset_szkice/Akceptacja_button.png"));
    offerDialogue.buttonWidth = acceptTexture->getWidth();
    offerDialogue.buttonHeight = acceptTexture->getHeight();
    offerDialogue.acceptButtonPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(acceptTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );

    auto&& dismissTexture = makeTexture(std::string_view("textures/Asset_szkice/Odrzucenie_button.png"));
    offerDialogue.dismissButtonPipeline = renderer.getPipelineManager()->create(
            gfx::pipeline::Pipeline::Desc{
                .vertexShaderPath = "shaders/vertex_default.glsl",
                .fragmentShaderPath = "shaders/fragment_default.glsl",
                .textures = {std::move(dismissTexture)},
                .buffers = {defaultUniformBuffer()},
            }
    );


    offerDialogue.acceptButtonX = offerDialogue.buttonWidth / 2;
    offerDialogue.acceptButtonY = windowHeight - offerDialogue.buttonHeight / 2;
    offerDialogue.buttonScaleX = 0.8;
    offerDialogue.buttonScaleY = 0.8;

    auto&& accept = scene.newEntity();
    accept.addComponent(
        scene::components::TransformComponent{
            .position = {offerDialogue.acceptButtonX, offerDialogue.acceptButtonY, -0.7},
            .rotation = {0, 0, 0, 1},
            .scale = {offerDialogue.buttonWidth * offerDialogue.buttonScaleX, offerDialogue.buttonHeight * offerDialogue.buttonScaleY, 0}
        }
    );
    auto mesh1 = makeMesh(defaultCenterVertices(), defaultIndices());
    accept.addComponent(
        scene::components::MeshComponent{
            .mesh = mesh1,
            .pipeline = offerDialogue.acceptButtonPipeline
        }
    );
    accept.addComponent(AcceptButtonFlag{});

    offerDialogue.dismissButtonX = offerDialogue.buttonWidth;
    offerDialogue.dismissButtonY = windowHeight - offerDialogue.buttonHeight;

    auto&& dismiss = scene.newEntity();
    dismiss.addComponent(
        scene::components::TransformComponent{
            .position = {offerDialogue.dismissButtonX, offerDialogue.dismissButtonY, -0.7},
            .rotation = {0, 0, 0, 1},
            .scale = {offerDialogue.buttonWidth * offerDialogue.buttonScaleX, offerDialogue.buttonHeight * offerDialogue.buttonScaleY, 0}
        }
    );
    auto mesh2 = makeMesh(defaultCenterVertices(), defaultIndices());
    dismiss.addComponent(
        scene::components::MeshComponent{
            .mesh = mesh2,
            .pipeline = offerDialogue.dismissButtonPipeline
        }
    );
    dismiss.addComponent(DismissButtonFlag{});
}

void OfferSystem::setup(Scene &scene) {
    auto&& manager = scene.newEntity();
    auto&& offerDialogue = manager.addComponent(OfferDialogue{});
    spawnOfferDialogue(scene);
}

void OfferSystem::update(Scene& scene) {}

}
