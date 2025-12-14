#include  <sound/SFX.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <SoundManager.h>

#include "sound/AudioDomain.h"

float SFX::gain = 0.5;

void SFX::playSFX(const std::string& path) {
    auto&& ambient = audioDomain.newEntity();
    auto&& ost = audioDomain.addComponent<audio::AudioSourceComponent>(ambient);
    // audio settings
    ost.gain = gain;
    ost.path = path;
    ost.isLooped = false;

    auto&& soundManager = audioDomain.global<SoundManager>();
    soundManager.audioManager->assignSource(ost);
    soundManager.audioManager->playSource(ost);
}