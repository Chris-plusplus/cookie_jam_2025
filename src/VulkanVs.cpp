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

#include "lifes/LifeManagerSystem.h"

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
	MultilineTextSystem::setup(*scene, textEntity, U"lorem\nipsum\ndupa", *font::FontDB::get()["Arial"]->regular(), {
		"shaders/text/fragment_atlas.glsl",
		"shaders/text/fragment_atlas_yellow.glsl",
		"shaders/text/fragment_atlas_blue.glsl"
		});

	// init physics system
	_physicsSystem = createUnique<physics::System>(scene->domain());

	// init SoundManager
	scene->domain().global<SoundManager>().init({explosionSoundPath});

	// explosion keys
	// auto&& leftVulkanKey = input::Keyboard::shiftLeft;
	// auto&& rightVulkanKey = input::Keyboard::enter;

	// volcano setup
	// auto vulkan1 = scene->newEntity();
	// VulkanSystem::setup(vulkan1, leftVulkanKey, false);
	// auto vulkan2 = scene->newEntity();
	// VulkanSystem::setup(vulkan2, rightVulkanKey, true);

	// AutoExplosionSystem::setup(vulkan1, input::Keyboard::one, {&leftVulkanKey, &rightVulkanKey});
	// AutoExplosionSystem::setup(vulkan2, input::Keyboard::two, {&leftVulkanKey, &rightVulkanKey});

	// ExplosionSystem::setupListener(*scene, vulkan1, vulkan2);

	// GroundSystem::setup(*scene);

	// CompetitionSystem::setup(*scene);

	// StaticTextSystem::setup(*scene,
	// 	U"Lewy shift",
	// 	*font::FontDB::get()["Arial"]->italic(), 100,
	// 	{10, vulkan1.getComponent<Vulkan>().particleOrigin.y + 25, -0.2f}
	// );
	// StaticTextSystem::setup(*scene,
	// 	U"Enter",
	// 	*font::FontDB::get()["Arial"]->italic(), 100,
	// 	{windowWidth - 300, vulkan2.getComponent<Vulkan>().particleOrigin.y + 25, -0.2f}
	// );

	now = std::chrono::high_resolution_clock::now();

	SlotMachineSystem::setup(*scene);

	LifeManagerSystem::setup(*scene);
}

using namespace std::chrono_literals;

void VulkanVs::update() {
	//static auto prevTime = std::chrono::high_resolution_clock::now();

	Ref<Scene> scene = scene::SceneManager::get()->currentScene();

	SlotMachineSystem::update(*scene);
	LifeManagerSystem::update(*scene);

	// update physics & transforms
	_physicsSystem->update();

	// update layer collisions
	//coll::LayerSystem::update(scene->domain());

	// update particles & explosions
	//ParticleSystem::update(scene->domain());
	//ExplosionSystem::updateText(scene->domain());

	// kill flagged entities
	//KillSystem::update(*scene);
	// remove layer collision flags
	//coll::LayerSystem::removeFlags(scene->domain());

	// update volcanos`
	//VulkanSystem::update(*scene);
	// update scores
	//CompetitionSystem::update(*scene);

	// update autoexplosion
	//AutoExplosionSystem::update(*scene);

	// synchronize audio
	scene->domain().global<SoundManager>().audioManager->synchronize(scene->domain());

	if (std::chrono::high_resolution_clock::now() - now > std::chrono::seconds(3)) {
		MultilineTextSystem::remove(*scene, textEntity);
	}

	//std::this_thread::sleep_for(16.6666ms - (std::chrono::high_resolution_clock::now() - prevTime));
	//prevTime = std::chrono::high_resolution_clock::now();
}

