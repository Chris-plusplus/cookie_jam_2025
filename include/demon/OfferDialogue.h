#pragma once
#include <archimedes/Ref.h>
#include <vector>
#include <archimedes/Gfx.h>

namespace demon {
    struct OfferDialogue {
        Ref<gfx::pipeline::Pipeline> acceptButtonPipeline;
        Ref<gfx::pipeline::Pipeline> dismissButtonPipeline;
        Ref<gfx::pipeline::Pipeline> containerPipeline;

        float buttonScaleX, buttonScaleY;
        float buttonWidth, buttonHeight;
        float containerWidth, containerHeight;
        float containerScaleX, containerScaleY;
        float containerX, containerY;
        float acceptButtonX, acceptButtonY;
        float dismissButtonX, dismissButtonY;
    };
}