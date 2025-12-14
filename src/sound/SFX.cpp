#include  <sound/SFX.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Defaults.h>
#include <Config.h>
#include <SoundManager.h>


void SFX::playSFX(Scene& scene, const std::string& path) {
    auto&& ambient = scene.newEntity();
    auto&& ost = ambient.addComponent<audio::AudioSourceComponent>();
    // audio settings
    ost.path = path;
    ost.isLooped = false;

    auto&& soundManager = scene::SceneManager::get()->currentScene()->domain().global<SoundManager>();
    soundManager.audioManager->assignSource(ost);
    soundManager.audioManager->playSource(ost);
}

void SFX::update(Scene& scene) {}

void SFX::setup(Scene& scene) {
}