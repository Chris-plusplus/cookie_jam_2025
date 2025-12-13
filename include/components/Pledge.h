#pragma once

#include <archimedes/Mmath.h>
#include <chrono>
#include <archimedes/Gfx.h>
#include <archimedes/gfx/Texture.h>

using namespace arch;
using namespace std::chrono_literals;

struct PledgeAnimation {
	decltype(0.0s) waitTime;
	float animSpeed;
	float beginPos;
	float endPos;
};

struct PledgeAnimationEndCallback {
	std::function<void()> onAnimationEnd;
};

struct PledgeTextures {
	Ref<gfx::pipeline::Pipeline> noPawPipeline;
	Ref<gfx::pipeline::Pipeline> withPawPipeline;
	Ref<gfx::pipeline::Pipeline> darkPipeline;
	Ref<gfx::pipeline::Pipeline> pawPipeline;
	Ref<gfx::Texture> noPaw;
	Ref<gfx::Texture> withPaw;
	Ref<gfx::Texture> dark;
	Ref<gfx::Texture> paw;
};

struct PlegdeDarkFlag {
	static constexpr bool flagComponent = true;
};

struct PlegdePawFlag {
	static constexpr bool flagComponent = true;
};
