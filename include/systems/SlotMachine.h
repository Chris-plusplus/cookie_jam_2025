#pragma once

#include <archimedes/Scene.h>

using namespace arch;

struct SlotMachineSystem {
	static void setup(Scene& scene);

	static void updateAnimation(Scene& scene);
	static void update(Scene& scene);
};