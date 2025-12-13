#include <demon/DemonManager.h>
#include <PointsCounter.h>
#include <demon/demon1.h>
#include <demon/demon2.h>
#include <demon/demon3.h>
#include <demon/OfferSystem.h>

std::mt19937 DemonManager::rng{std::random_device{}()};

int DemonManager::currentRoll=0;
bool DemonManager::active_demon = false;
int DemonManager::roll_to_demon = std::uniform_int_distribution<int>{4, 6}(rng);
int DemonManager::roll_to_demon2 = std::uniform_int_distribution<int>{1, 12}(rng);
int DemonManager::points_to_demon = std::uniform_int_distribution<int>{2000, 3000}(rng);

void DemonManager::setup(Scene& scene) {
}

void DemonManager::update(Scene& scene) {

    if (!active_demon) {
        if (PointsCounter::score>=points_to_demon) {
            points_to_demon+=std::uniform_int_distribution<int>{2000, 3000}(rng);
            active_demon=true;
            demon2::show(scene);
            demon::OfferSystem::spawnOfferDialogue(scene, "dupa1");
            // Logger::debug("demon2");
        }
        else if (currentRoll>=roll_to_demon) {
            roll_to_demon+=std::uniform_int_distribution<int>{4, 6}(rng);
            active_demon=true;
            demon1::show(scene);
            demon::OfferSystem::spawnOfferDialogue(scene, "dupa2");
            // Logger::debug("demon1");
        }
        else if (currentRoll>=roll_to_demon2) {
            roll_to_demon+=std::uniform_int_distribution<int>{1, 12}(rng);
            active_demon=true;
            demon3::show(scene);
            demon::OfferSystem::spawnOfferDialogue(scene, "dupa3");
            // Logger::debug("demon3");
        }
    }
}
void DemonManager::addroll() {
    currentRoll++;
}