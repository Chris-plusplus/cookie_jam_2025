#include <SoundManager.h>

namespace vs {

SoundManager::~SoundManager() {
	audioManager->stop();
	audioThread->request_stop();
	audioThread->join();
}

void SoundManager::init(const std::vector<std::string>& soundFiles) {
	audioManager = createUnique<audio::AudioManager>(&soundBank);
	audioThread = createUnique<std::jthread>(&audio::AudioManager::play, audioManager.get());
	for (auto&& soundFile : soundFiles) {
		soundBank.addClip(soundFile);
	}
	soundBank.loadInitialGroups();
}

}