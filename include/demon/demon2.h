#pragma once
#include <archimedes/Scene.h>

using namespace arch;

class demon2 {
public:
    static void show(Scene& scene);
    static void hide(Scene& scene);
    static void setup(Scene& scene);
};

struct Demon2Flag {
    static constexpr bool flagComponent = true;
};