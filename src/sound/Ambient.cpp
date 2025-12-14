#include  <sound/Ambient.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <SoundManager.h>

#include "sound/CurrentAmbientFlag.h"
#include "sound/AudioDomain.h"

void Ambient::setAmbient(const std::string& path) {
    auto&& soundManager = audioDomain.global<SoundManager>();
    auto&& ambient = audioDomain.newEntity();
    auto&& ost = audioDomain.addComponent<audio::AudioSourceComponent>(ambient);
    audioDomain.addComponent<CurrentAmbientFlag>(ambient);
    // audio settings
    ost.path = path;
    ost.isLooped = true;

    soundManager.audioManager->assignSource(ost);
    soundManager.audioManager->playSource(ost);
    Logger::debug("Ambient set {}", path);
}

void Ambient::stopAmbient() {
    auto&& soundManager = audioDomain.global<SoundManager>();
    auto&& oldAmbient = audioDomain.view<CurrentAmbientFlag>().front();
    auto&& oldOst = audioDomain.getComponent<audio::AudioSourceComponent>(oldAmbient);
    soundManager.audioManager->stopSource(oldOst);
    audioDomain.kill(oldAmbient);
}