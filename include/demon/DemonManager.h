#pragma once
#include <archimedes/Scene.h>
#include <random>
#include <demon/OfferSystem.h>

using namespace arch;

class DemonManager {
public:
    static void setup(Scene& scene);
    static void update(Scene& scene);
    static demon::DemonType active_demon;
    static int points_to_demon;
    static int roll_to_demon;
    static std::mt19937 rng;
    static int currentRoll;
    static int roll_to_demon2;
    static void addroll();
    static void hide(Scene& scene);
    static bool isBlocked;
};