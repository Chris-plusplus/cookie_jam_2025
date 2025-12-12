#pragma once

#include <archimedes/Scene.h>
#include <archimedes/Text.h>

using namespace arch;


// Creates static text
struct StaticTextSystem {
	static void setup(Scene& scene, const std::u32string& text, font::Face& font, f32 fontSize, float3 pos, f32 angle = 0);
};

