#pragma once

#include <archimedes/Scene.h>
#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotMachineSystem {
	static void setup(Scene& scene);

	static bool isEnd;

	static void updateAnimation(Scene& scene);
	static void update(Scene& scene);

	static slots::RewardType reward(Scene& scene);
	static const std::vector<int>& drawn(Scene& scene);

	static void onDrawn(Scene& scene, std::function<int(Scene&, const std::vector<int>&)> event);
};