#pragma once

namespace vs {

// Component which stores the max particles of a player, also flags the "Max" text entity
template<bool IsRight>
struct CompetitionMaxParticles {
	u32 value = 0;
};

// Flags the "Current" text entity
template<bool IsRight>
struct CompetitionCurrentTextFlag {
	static constexpr bool flagComponent = true;
};

}