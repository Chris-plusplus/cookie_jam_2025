#include <VulkanVs.h>
#include <MakeTexture.h>
#include <archimedes/Font.h>
#include <archimedes/Scene.h>
#include <systems/Kill.h>
#include <Config.h>
#include <SoundManager.h>
#include <archimedes/text/TextComponent.h>
#include <Defaults.h>
#include <systems/StaticText.h>
#include <systems/MultilineText.h>
#include <systems/SlotMachine.h>

ecs::Entity textEntity;
decltype(std::chrono::high_resolution_clock::now()) now;

void VulkanVs::init() {
	// load font
	font::FontDB::get()["Arial"]->bold()->assure();
	font::FontDB::get()["Arial"]->regular()->assure();

	// make scene
	Ref<Scene> scene = createRef<Scene>();
	scene::SceneManager::get()->changeScene(scene);

	auto textEnt = scene->newEntity();
	textEntity = textEnt.handle();
	textEnt.addComponent(
		scene::components::TransformComponent{
			.position = {100, 500, -0.3},
			.rotation = {0, 0, 0, 1},
			.scale = {100, 100, 0}
		}
	);
	MultilineTextSystem::setup(*scene, textEntity, U"lorem\nipsum\ndupa", *font::FontDB::get()["Arial"]->regular());

	// init physics system
	_physicsSystem = createUnique<physics::System>(scene->domain());

	// init SoundManager
	scene->domain().global<SoundManager>().init({explosionSoundPath});

	// explosion keys
	auto&& leftVulkanKey = input::Keyboard::shiftLeft;
	auto&& rightVulkanKey = input::Keyboard::enter;

	// volcano setup
	auto vulkan1 = scene->newEntity();
	auto vulkan2 = scene->newEntity();

	now = std::chrono::high_resolution_clock::now();

	SlotMachineSystem::setup(*scene);
}

void VulkanVs::update() {
	Ref<Scene> scene = scene::SceneManager::get()->currentScene();

	SlotMachineSystem::update(*scene);

	// update physics & transforms
	_physicsSystem->update();

	// update layer collisions

	// update particles & explosions

	// kill flagged entities
	KillSystem::update(*scene);
	// remove layer collision flags

	// update volcanos`
	// update scores

	// update autoexplosion

	// synchronize audio
	scene->domain().global<SoundManager>().audioManager->synchronize(scene->domain());

	if (std::chrono::high_resolution_clock::now() - now > std::chrono::seconds(3)) {
		MultilineTextSystem::remove(*scene, textEntity);
	}
}

