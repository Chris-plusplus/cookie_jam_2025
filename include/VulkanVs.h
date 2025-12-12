#pragma once

#include <archimedes/Application.h>
#include <archimedes/physics/System.h>
#include <archimedes/Ref.h>

using namespace arch;

namespace vs {

// VulkanVs game
class VulkanVs: public Application {
public:
	VulkanVs() = default;

	void init() override;
	void update() override;

private:
	Ref<physics::System> _physicsSystem;
};

}
