#pragma once

#include <archimedes/Scene.h>

using namespace arch;

struct Settings {
    static int Music;
    static int SFX;
    static void setup(Scene& scene);
    static void exit(Scene& scene);
    static void update(Scene& scene);
};
