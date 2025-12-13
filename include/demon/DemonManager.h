#pragma once
#include <archimedes/Scene.h>
#include <random>

using namespace arch;

class DemonManager {
public:
    static void setup(Scene& scene);
    static void update(Scene& scene);
    static bool active_demon;
    static int points_to_demon;
    static int roll_to_demon;
    static std::mt19937 rng;
    static int currentRoll;
    static int roll_to_demon2;
    static void addroll();
};