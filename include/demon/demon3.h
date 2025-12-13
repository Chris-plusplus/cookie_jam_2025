#pragma once
#include <archimedes/Scene.h>

using namespace arch;

class demon3 {
public:
    static void show(Scene& scene);
    static void hide(Scene& scene);
    static void setup(Scene& scene);
};

struct Demon3Flag {
    static constexpr bool flagComponent = true;
};