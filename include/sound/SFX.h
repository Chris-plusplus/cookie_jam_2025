#pragma once
#include <archimedes/Scene.h>

using namespace arch;

struct SFX {
    static void setup(Scene& scene);

    static void update(Scene& scene);

    static void playSFX(Scene& scene, const std::string& path);
};