#pragma once
#include <archimedes/Scene.h>

using namespace arch;

class demon1 {
public:
	static void show(Scene& scene);
	static void hide(Scene& scene);
	static void setup(Scene& scene);
};

struct Demon1Flag {
	static constexpr bool flagComponent = true;
};