#pragma once

#include <archimedes/Ref.h>
#include <vector>
#include <archimedes/Gfx.h>
#include <archimedes/Scene.h>
#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotMachine {
	std::vector<Ref<gfx::pipeline::Pipeline>> symbols;
	std::vector<std::vector<ecs::Entity>> slotsByCollumn;
	float upperBound;
	float lowerBound;
	u32 slotCount = 3;

	bool strideCompute = false;
	bool stride = false;
	bool slotAnimation = false;

	float leverAnimationSpeed = 0;
	float leverAnimation = 0;

	float pawAnimationSpeed = 0;
	float pawAnimation = 0;

	std::vector<int> drawn;
	std::function<int(Scene&, const std::vector<int>&)> onDrawn;
};