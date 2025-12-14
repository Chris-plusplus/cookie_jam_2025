#include <demon/SwitchSystem.h>
#include <demon/PositiveSwitch.h>
#include <demon/NegativeSwitch.h>
#include <ranges>
#include <random>

auto rng = std::mt19937(std::random_device{}());

int SwitchSystem::getSwitch(Scene& scene) {

	auto&& positiveSwitch = scene.domain().global<PositiveSwitch>();
	auto&& negativeSwitch = scene.domain().global<NegativeSwitch>();

	std::vector<float> chances;
	if (positiveSwitch.chance != 0) {
		chances.push_back(positiveSwitch.chance);
	}
	if (negativeSwitch.chance != 0) {
		chances.push_back(-negativeSwitch.chance);
	}
	if (chances.size() == 0) {
		return 0;
	}

	std::ranges::shuffle(chances, rng);

	int result = chances.front();

	return std::uniform_real_distribution(0.f, 1.f)(rng) <= std::fabs(chances.front()) ? result : 0;
}

int SwitchSystem::indexToSwitch(int size) {
	return std::uniform_int_distribution(0, size)(rng);
}

