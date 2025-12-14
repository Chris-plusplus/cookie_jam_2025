#pragma once

#include <archimedes/audio/AudioManager.h>
#include <archimedes/audio/SoundDevice.h>
#include <archimedes/Ref.h>

using namespace arch;


struct SoundManager {
	audio::SoundDevice device;
	audio::SoundBank soundBank;
	Unique<audio::AudioManager> audioManager;
	Unique<std::jthread> audioThread;

	~SoundManager();

	void init();
};

