#include  <sound/Ambient.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <SoundManager.h>

#include "sound/CurrentAmbientFlag.h"

void Ambient::setAmbient(Scene& scene, const std::string& path) {
    auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
    auto&& ambient = scene.newEntity();
    auto&& ost = ambient.addComponent<audio::AudioSourceComponent>();
    ambient.addComponent<CurrentAmbientFlag>();
    // audio settings
    ost.path = path;
    ost.isLooped = true;

    soundManager.audioManager->assignSource(ost);
    soundManager.audioManager->playSource(ost);
}

void Ambient::stopAmbient(Scene &scene) {
    auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
    auto&& oldAmbient = scene.domain().view<CurrentAmbientFlag>().front();
    auto&& oldOst = scene.domain().getComponent<audio::AudioSourceComponent>(oldAmbient);
    soundManager.audioManager->stopSource(oldOst);
    scene.domain().kill(oldAmbient);
}

void Ambient::update(Scene& scene) {}

void Ambient::setup(Scene& scene) {
    setAmbient(scene, "main_theme.ogg");
}