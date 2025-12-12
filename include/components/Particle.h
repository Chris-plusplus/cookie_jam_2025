#pragma once

#include <archimedes/Mmath.h>

namespace vs {

// Particle component
template<bool IsRight>
struct Particle {
	// Quaternion which rotates particle
	glm::quat rotationQuat;
	// "bounceiness" of particle
	float e;
	u32 collisionCounter = 0;
};

}