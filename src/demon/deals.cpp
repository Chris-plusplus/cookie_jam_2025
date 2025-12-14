#include "demon/deals.h"
#include <lifes/LifeManager.h>
#include <archimedes/scene/Scene.h>
#include <demon/PositiveSwitch.h>
#include <demon/NegativeSwitch.h>
#include <slots/SlotGlitchChance.h>
#include <demon/SwitchSystem.h>

namespace Deals {
int sachet = 1;
bool positive_change = false;
bool negative_change = false;
int bad_chance = 1;
slots::RewardType negative_points = slots::RewardType::_none;
bool glitch = false;
bool no_gurken = false;
int hp_chanse = 1;
bool better_food = false;
}

std::mt19937 Deal::rng{std::random_device{}()};

void Deal::big_sachet(Scene& scene) {
	Deals::sachet *= 1.1;
	auto&& rewardGenerator = scene.domain().global<slots::RewardGenerator>();
	rewardGenerator.multiplyProbability(slots::RewardType::sachet, 1.1f);
}
void Deal::robber() {
	int temp = std::uniform_int_distribution<int>{2, 5}(rng);
	Deals::negative_points = (slots::RewardType)temp;
}
void Deal::quality() {
	Deals::better_food = true;
}
void Deal::luck_down(Scene& scene) {
	Deals::bad_chance *= 1.1;
	auto&& rewardGenerator = scene.domain().global<slots::RewardGenerator>();
	rewardGenerator.multiplyProbability(slots::RewardType::dry, 1.1f);
}
void Deal::nolife(Scene& scene) {
	auto&& manager = scene.domain().view<LifeManager>().front();
	auto&& lifeManager = scene.domain().getComponent<LifeManager>(manager);
	lifeManager.updateLifes(-2);
}
void Deal::set_glitch(Scene& scene) {
	Deals::glitch = true;
	scene.domain().global<SlotGlitchChance>().value = 0.20;
}
void Deal::set_negative_change(Scene& scene) {
	Deals::negative_change = true;
	SwitchSystem::addEffect<NegativeSwitch>(scene, 1.0);
}
void Deal::set_positive_change(Scene& scene) {
	Deals::positive_change = true;
	SwitchSystem::addEffect<PositiveSwitch>(scene, 1.0);
}
void Deal::set_no_gurken() {
	Deals::no_gurken = true;
}
void Deal::short_on_life(Scene& scene) {
	Deals::hp_chanse *= 0.9;
	auto&& rewardGenerator = scene.domain().global<slots::RewardGenerator>();
	rewardGenerator.multiplyProbability(slots::RewardType::coin, 0.9f);
}




