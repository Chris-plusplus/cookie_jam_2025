#pragma once

#include <archimedes/Scene.h>
#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotMachineSystem {
	static void setup(Scene& scene);

	static void updateAnimation(Scene& scene);
	static void update(Scene& scene);

	static slots::RewardType reward(Scene& scene);
	static const std::vector<slots::RewardType>& drawn(Scene& scene);
};