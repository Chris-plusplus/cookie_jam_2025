#include <lifes/LifeManager.h>

void LifeManager::updateLifes(int difference) {
	currentLifes = std::clamp(currentLifes + difference, 0, maxLifes);
	Logger::debug("currentLifes = {}", currentLifes);
}
