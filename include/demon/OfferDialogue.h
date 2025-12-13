#pragma once
#include <archimedes/Ref.h>
#include <vector>
#include <archimedes/Gfx.h>

namespace demon {
    struct OfferDialogue {
        Ref<gfx::pipeline::Pipeline> acceptButtonPipeline;
        Ref<gfx::pipeline::Pipeline> dismissButtonPipeline;

        float buttonScaleX, buttonScaleY;
        float buttonWidth, buttonHeight;
        float acceptButtonX, acceptButtonY;
        float dismissButtonX, dismissButtonY;
    };
}