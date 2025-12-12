#pragma once

#include <archimedes/Mmath.h>
#include <archimedes/Input.h>

using namespace arch;

// look for Config.cpp, where values can be set

extern u32 windowWidth;
extern u32 windowHeight;

// makes layers easier to distinguish
struct Layer {
	enum Value {
		ground,
		particle
	};
};

extern std::string explosionSoundPath;

