#pragma once

#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotObject {
	float speed{};
	float acceleration{};
	float jolt{};
	float minSpeed = -2.5;
	float maxSpeed = -75;
	slots::RewardType type;
};