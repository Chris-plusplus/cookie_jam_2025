#pragma once

#include <archimedes/Scene.h>
#include <archimedes/Text.h>

using namespace arch;

struct MultilineTextSystem {
	static void setup(Scene& scene, ecs::Entity parent, const std::u32string& text, font::Face& font, const std::vector<std::string>& shaders = {"shaders/text/fragment_atlas.glsl"});
	static void remove(Scene& scene, ecs::Entity parent);
};