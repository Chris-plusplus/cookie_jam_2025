#pragma once

#include <archimedes/Scene.h>

using namespace arch;


// behavior for competition mechanism (score handling)
struct CompetitionSystem {
	static void setup(Scene& scene);
	static void update(Scene& scene);
};

