#include <demon/DemonManager.h>
#include <PointsCounter.h>
#include <demon/demon1.h>
#include <demon/demon2.h>
#include <demon/demon3.h>
#include <demon/OfferSystem.h>

std::mt19937 DemonManager::rng{std::random_device{}()};

int DemonManager::currentRoll = 0;
demon::DemonType DemonManager::active_demon = demon::DemonType::_none;
int DemonManager::roll_to_demon = std::uniform_int_distribution<int>{4, 6}(rng);
int DemonManager::roll_to_demon2 = std::uniform_int_distribution<int>{1, 12}(rng);
int DemonManager::points_to_demon = std::uniform_int_distribution<int>{2000, 3000}(rng);
bool DemonManager::isBlocked = false;

void DemonManager::setup(Scene& scene) {
}

void DemonManager::update(Scene& scene) {

	if (active_demon == demon::DemonType::_none && !isBlocked) {
		if (PointsCounter::score >= points_to_demon) {
			points_to_demon += std::uniform_int_distribution<int>{2000, 3000}(rng);
			active_demon = demon::DemonType::demon2;
			demon2::show(scene);
			demon::OfferSystem::spawnOfferDialogue(scene, "dupa1");
			Logger::debug("demon2");
		} else if (currentRoll >= roll_to_demon) {
			roll_to_demon += std::uniform_int_distribution<int>{4, 6}(rng);
			active_demon = demon::DemonType::demon1;
			demon1::show(scene);
			demon::OfferSystem::spawnOfferDialogue(scene, "dupa2");
			Logger::debug("demon1");
		} else if (currentRoll >= roll_to_demon2) {
			roll_to_demon2 += std::uniform_int_distribution<int>{1, 12}(rng);
			active_demon = demon::DemonType::demon3;
			demon3::show(scene);
			demon::OfferSystem::spawnOfferDialogue(scene, "dupa3");
			Logger::debug("demon3");
		}
		isBlocked = true;
	}
}

void DemonManager::hide(Scene &scene) {
	Logger::debug("remove demon");
	switch (active_demon) {
		case demon::DemonType::demon1:
			demon1::hide(scene);
			demon::OfferSystem::clearOfferDialogue(scene);
			active_demon = demon::DemonType::_none;
			Logger::debug("Remove demon1");
			return;
		case demon::DemonType::demon2:
			demon2::hide(scene);
			demon::OfferSystem::clearOfferDialogue(scene);
			active_demon = demon::DemonType::_none;
			Logger::debug("Remove demon2");
			return;
		case demon::DemonType::demon3:
			demon3::hide(scene);
			demon::OfferSystem::clearOfferDialogue(scene);
			active_demon = demon::DemonType::_none;
			Logger::debug("Remove demon3");
			return;
		case demon::DemonType::_none:
			Logger::debug("remove none");
			return;
	}
}

void DemonManager::addroll() {
	currentRoll++;
}