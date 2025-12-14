#pragma once
#include "Defaults.h"
#include <archimedes/Scene.h>
#include <slots/RewardGenerator.h>
#include <random>

namespace Deals {
extern int sachet;
extern bool positive_change;
extern bool negative_change;
extern int bad_chance;
extern slots::RewardType negative_points;
extern bool glitch;
extern bool no_gurken;
extern int hp_chanse;
extern bool better_food;
}

class Deal {
public:
	static void big_sachet(Scene& scene);
	static void set_positive_change(Scene& scene);
	static void set_negative_change(Scene& scene);
	static void set_glitch(Scene& scene);
	static void set_no_gurken();
	static void luck_down(Scene& scene);
	static void short_on_life(Scene& scene);
	static void nolife(Scene& scene);
	static void quality();
	static void robber();
	static std::mt19937 rng;
};