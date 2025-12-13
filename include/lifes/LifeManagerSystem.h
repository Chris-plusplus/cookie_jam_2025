#pragma once

#include <archimedes/Scene.h>

using namespace arch;

struct LifeManagerSystem {
    static void setup(Scene& scene);

    static void update(Scene& scene);

    static void drawCoins(Scene& scene);

    static void updateLifes(Scene& scene, int difference);
};