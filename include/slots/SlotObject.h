#pragma once

#include <slots/RewardGenerator.h>

using namespace arch;

struct SlotObject {
	float speed{};
	float acceleration{};
	float jolt{};
	float minSpeed = -25;
	float maxSpeed = -5000;
	slots::RewardType type;
};