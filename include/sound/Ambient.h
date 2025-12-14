#pragma once
#include <archimedes/Scene.h>

using namespace arch;

struct Ambient {

    static void setup(Scene& scene);

    static void update(Scene& scene);

    static void setAmbient(Scene& scene, const std::string& path);

    static void stopAmbient(Scene& scene);
};