#pragma once

#include <archimedes/Scene.h>
#include <archimedes/Text.h>

using namespace arch;

struct MultilineTextSystem {
	static void setup(Scene& scene, ecs::Entity parent, const std::u32string& text, font::Face& font);
	static void remove(Scene& scene, ecs::Entity parent);
};