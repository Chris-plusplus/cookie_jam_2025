#include  <sound/SFX.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <SoundManager.h>

#include "sound/AudioDomain.h"


void SFX::playSFX(const std::string& path) {
    auto&& ambient = audioDomain.newEntity();
    auto&& ost = audioDomain.addComponent<audio::AudioSourceComponent>(ambient);
    // audio settings
    ost.path = path;
    ost.isLooped = false;

    auto&& soundManager = audioDomain.global<SoundManager>();
    soundManager.audioManager->assignSource(ost);
    soundManager.audioManager->playSource(ost);
}