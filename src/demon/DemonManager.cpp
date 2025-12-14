#include <demon/DemonManager.h>
#include <PointsCounter.h>
#include <demon/demon1.h>
#include <demon/demon2.h>
#include <demon/demon3.h>
#include <demon/OfferSystem.h>
#include <systems/SlotMachine.h>

std::mt19937 DemonManager::rng{std::random_device{}()};

int DemonManager::currentRoll = 0;
demon::DemonType DemonManager::active_demon = demon::DemonType::_none;
int DemonManager::roll_to_demon = std::uniform_int_distribution<int>{3, 5}(rng);
int DemonManager::roll_to_demon2 = std::uniform_int_distribution<int>{1, 12}(rng);
int DemonManager::points_to_demon = std::uniform_int_distribution<int>{1000, 2000}(rng);
bool DemonManager::isBlocked = false;

void DemonManager::setup(Scene& scene) {}

void DemonManager::update(Scene& scene) {

	if (active_demon == demon::DemonType::_none && !isBlocked && !SlotMachineSystem::isEnd) {
		// smutny
		if (PointsCounter::score >= points_to_demon) {
			points_to_demon += std::uniform_int_distribution<int>{2000, 3000}(rng);
			active_demon = demon::DemonType::demon2;
			demon2::show(scene);
			std::string flavour = "Witaj. Mam dla ciebie\npropozycję nie do odrzucenia. \n";
			int temp = std::uniform_int_distribution<int>{1, 2}(rng);
			switch (temp) {
				case 1:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś zwiększyć wartość suchej\nkarmy za negatywne punkty z\nlosowej nagrody?", demon::OfferType::d21);
					break;
				case 2:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś zwiększyć wartość suchej\nkarmy kosztem szansy na\nutracenie wylosowanego jackpota?", demon::OfferType::d22);
					break;
			}
			Logger::debug("demon2");
		} else if (currentRoll >= roll_to_demon) {
			// cool
			roll_to_demon += std::uniform_int_distribution<int>{5, 7}(rng);
			active_demon = demon::DemonType::demon1;
			std::string flavour = "Siema, tego ten.\nMoże zechcesz podpisać mały pakt? \n";
			demon1::show(scene);
			int temp = std::uniform_int_distribution<int>{1, 4}(rng);
			switch (temp) {
				case 1:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś zwiększyć szanse na\nwylosowanie dobrej karmy za\ntrochę życia?", demon::OfferType::d11);
					break;
				case 2:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś zwiększyć szanse na\nwylosowanie dobrej karmy\noraz suchej karmy?", demon::OfferType::d12);
					break;
				case 3:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś zwiększyć szanse na\nwylosowanie jackpota za\ntrochę życia?", demon::OfferType::d13);
					break;
				case 4:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś zwiększyć szanse na\nwylosowanie jackpota oraz\nsuchej karmy?", demon::OfferType::d14);
					break;
			}
			Logger::debug("demon1");
		} else if (currentRoll >= roll_to_demon2) {
			// distorted
			roll_to_demon2 += std::uniform_int_distribution<int>{1, 14}(rng);
			active_demon = demon::DemonType::demon3;
			std::string flavour = "Per i Kele witają cię.\nCo powiesz na taki korzystny pakt? \n";
			demon3::show(scene);
			int temp = std::uniform_int_distribution<int>{1, 2}(rng);
			switch (temp) {
				case 1:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Chciałbyś dostać szansę na\nzgliczowanie maszyny? (reroll)", demon::OfferType::d31);
					break;
				case 2:
					demon::OfferSystem::spawnOfferDialogue(scene, flavour + "Mogę uodpornić cię na większość\nogórków ale w zamian rzadziej\nbędą pojawiać się życia.", demon::OfferType::d32);
					break;
			}
			Logger::debug("demon3");
		}
		isBlocked = true;
	}
}

void DemonManager::hide(Scene& scene) {
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