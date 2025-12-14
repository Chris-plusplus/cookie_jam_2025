#include <SoundManager.h>
#include <filesystem>

namespace fs = std::filesystem;

SoundManager::~SoundManager() {
	audioManager->stop();
	audioThread->request_stop();
	audioThread->join();
}

void SoundManager::init() {
	std::vector<std::string> soundFiles;
	for (const auto& entry : fs::directory_iterator("sounds")) {
		fs::path entryPath = entry.path();
		soundFiles.emplace_back(entryPath.filename().string());
		Logger::debug("{}", entryPath.filename().string());
	}
	for (auto&& soundFile : soundFiles) {
		soundBank.addClip(soundFile);
	}
	soundBank.loadInitialGroups();
	start();
}

void SoundManager::start() {
	audioManager = createUnique<audio::AudioManager>(&soundBank);
	audioThread = createUnique<std::jthread>(&audio::AudioManager::play, audioManager.get());
}

void SoundManager::kill() {
	audioManager->stop();
	audioThread->request_stop();
	audioThread->join();
}

