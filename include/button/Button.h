#pragma once

#include <functional>
#include <archimedes/Mmath.h>
#include <archimedes/Scene.h>

using namespace arch;

struct Button {
	struct InactiveFlag {
		static constexpr bool flagComponent = true;
	};

	float2 topLeft;
	float2 bottomRight;
	Ref<gfx::pipeline::Pipeline> onNotHover;
	Ref<gfx::pipeline::Pipeline> onHover;

	std::function<void(Scene&, ecs::Entity)> callback;
};
