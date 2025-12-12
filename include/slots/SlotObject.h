#pragma once

#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotObject {
	float speed{};
	float acceleration{};
	float jolt{};
	float minSpeed = -5;
	float maxSpeed = -100;
	slots::RewardType type;
};