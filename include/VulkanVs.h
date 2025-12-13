#pragma once

#include <archimedes/Application.h>
#include <archimedes/physics/System.h>
#include <archimedes/Ref.h>

using namespace arch;


// VulkanVs game
class VulkanVs: public Application {
public:
	VulkanVs() = default;
	~VulkanVs();

	void init() override;
	void update() override;
};

