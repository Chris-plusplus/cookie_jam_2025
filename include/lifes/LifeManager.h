#pragma once

#include <archimedes/Ref.h>
#include <vector>
#include <archimedes/Gfx.h>

using namespace arch;

struct LifeManager {
    Ref<gfx::pipeline::Pipeline> containerPipeline;
    Ref<gfx::pipeline::Pipeline> emptyCoinPipeline;
    Ref<gfx::pipeline::Pipeline> fullCoinPipeline;

    int currentLifes = 9;
    int maxLifes = 9;
};