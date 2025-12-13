#pragma once

#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotObject {
	float speed{};
	float acceleration{};
	float jolt{};
	float minSpeed = -10;
	float maxSpeed = -50;
	slots::RewardType type;
};