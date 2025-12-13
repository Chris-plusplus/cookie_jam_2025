#pragma once

#include <archimedes/Ref.h>
#include <vector>
#include <archimedes/Gfx.h>

using namespace arch;

struct SlotMachine {
	std::vector<Ref<gfx::pipeline::Pipeline>> symbols;
	std::vector<std::vector<ecs::Entity>> slotsByCollumn;
	float upperBound;
	float lowerBound;
	u32 slotCount = 3;

	float leverAnimationSpeed = 0;
	float leverAnimation = 0;
};