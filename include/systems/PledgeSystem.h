#pragma once

#include <archimedes/Scene.h>

using namespace arch;

struct PledgeSystem {
	static void setup(Scene& scene);
	static void update(Scene& scene);
	static void cleanup(Scene& scene);

	static void setCallback(Scene& scene, std::function<void()> callback);
};