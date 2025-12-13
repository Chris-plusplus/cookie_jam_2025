#pragma once

#include <archimedes/Scene.h>
#include "Switch.h"

using namespace arch;

struct SwitchSystem {
	template<std::derived_from<Switch> T>
	static void addEffect(Scene& scene, float chance) {
		scene.domain().global<T>().chance = chance;
	}

	static int getSwitch(Scene& scene);

	static int indexToSwitch(int size);
};