#pragma once
#include <archimedes/Scene.h>

using namespace arch;

struct SFX {

    static float gain;
    static void playSFX(const std::string& path);
};