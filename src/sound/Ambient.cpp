#include  <sound/Ambient.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <SoundManager.h>

void Ambient::initAmbients(Scene& scene) {
    auto&& ambient = scene.newEntity();
    auto&& ost = ambient.addComponent<audio::AudioSourceComponent>();
    // audio settings
    ost.path = "main_theme.ogg";
    ost.isLooped = true;

    auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
    soundManager.audioManager->assignSource(ost);
    soundManager.audioManager->playSource(ost);
}

void Ambient::update(Scene& scene) {}

void Ambient::setup(Scene& scene) {
    initAmbients(scene);
}