#pragma once

#include <archimedes/Scene.h>

using namespace arch;

struct ButtonSystem {
	static void setup(Scene& scene, ecs::Entity entity, float2 bottonLeft, float2 topRight, Ref<gfx::pipeline::Pipeline> onNotHover, Ref<gfx::pipeline::Pipeline> onHover, std::function<void(Scene&, ecs::Entity)> callback);
	static void update(Scene& scene);
};