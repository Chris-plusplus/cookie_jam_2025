#pragma once

#include <archimedes/Scene.h>

using namespace arch;

struct EndingSystem {
	static void badEnd(Scene& scene, std::string_view texturePath, std::string_view endingTheme);
};