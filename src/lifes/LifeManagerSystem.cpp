#include <lifes/LifeManagerSystem.h>
#include <lifes/LifeManager.h>
#include <Defaults.h>
#include <MakeTexture.h>
#include <MakeMesh.h>
#include <Config.h>
#include <fstream>
#include <archimedes/Input.h>
#include <lifes/LifeFlag.h>


void LifeManagerSystem::drawCoins(Scene& scene) {
	auto&& manager = scene.domain().view<LifeManager>().front();
	auto&& lifeManager = scene.domain().getComponent<LifeManager>(manager);

	std::vector<ecs::Entity> toRemove;
	for (auto&& entity : scene.domain().view<LifeFlag>()) {
		toRemove.push_back(entity);
	}
	Logger::debug("removing");
	for (auto&& entity : toRemove) {
		Logger::debug("{:v}", entity);
		scene.domain().kill(entity);
	}

	float x = lifeManager.containerX / 2 - lifeManager.coinWidth * lifeManager.coinScaleX * 0.75;
	float y = lifeManager.containerY;
	for (int i = 0; i < lifeManager.currentLifes; i++) {
		auto&& coin = scene.newEntity();
		auto&& transform = coin.addComponent(
			scene::components::TransformComponent{
				.position = {x, y, -0.6},
				.rotation = {0, 0, 0, 1},
				.scale = {lifeManager.coinWidth * lifeManager.coinScaleX, lifeManager.coinHeight * lifeManager.coinScaleY, 0}
			}
		);
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto&& meshComp = coin.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = lifeManager.fullCoinPipeline
			}
		);
		auto&& flag = coin.addComponent(LifeFlag{});
		x += lifeManager.coinWidth * lifeManager.coinScaleX;
	}
	for (int i = 0; i < lifeManager.maxLifes - lifeManager.currentLifes; i++) {
		auto&& coin = scene.newEntity();
		auto&& transform = coin.addComponent(
			scene::components::TransformComponent{
				.position = {x, y, -0.6},
				.rotation = {0, 0, 0, 1},
				.scale = {lifeManager.coinWidth * lifeManager.coinScaleX, lifeManager.coinHeight * lifeManager.coinScaleY, 0}
			}
		);
		auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
		auto&& meshComp = coin.addComponent(
			scene::components::MeshComponent{
				.mesh = mesh,
				.pipeline = lifeManager.emptyCoinPipeline
			}
		);
		auto&& flag = coin.addComponent(LifeFlag{});
		x += lifeManager.coinWidth * lifeManager.coinScaleX;
	}
}

void LifeManagerSystem::setup(Scene& scene) {
	auto manager = scene.newEntity();
	auto&& lifeManager = manager.addComponent<LifeManager>();
	auto&& renderer = *gfx::Renderer::current();

	// init lifes container sprite
	float2 slotTexSize{};
	auto&& containerTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_bar_asset.png"));
	slotTexSize.x = containerTexture->getWidth();
	slotTexSize.y = containerTexture->getHeight();
	lifeManager.containerWidth = containerTexture->getWidth();
	lifeManager.containerHeight = containerTexture->getHeight();
	lifeManager.containerPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(containerTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);
	lifeManager.containerX = lifeManager.containerWidth / 2;
	lifeManager.containerY = windowHeight - lifeManager.containerHeight / 2;
	Logger::info("y = {}", lifeManager.containerY);
	lifeManager.containerScaleX = 0.9;
	lifeManager.containerScaleY = 0.9;
	lifeManager.coinScaleX = 0.8;
	lifeManager.coinScaleY = 0.8;
	auto&& transform = manager.addComponent(
		scene::components::TransformComponent{
			.position = {lifeManager.containerX, lifeManager.containerY, -0.5},
			.rotation = {0, 0, 0, 1},
			.scale = {lifeManager.containerWidth * lifeManager.containerScaleX, lifeManager.containerHeight * lifeManager.containerScaleY, 0}
		}
	);
	auto mesh = makeMesh(defaultCenterVertices(), defaultIndices());
	auto&& meshComp = manager.addComponent(
		scene::components::MeshComponent{
			.mesh = mesh,
			.pipeline = lifeManager.containerPipeline
		}
	);
	auto&& emptyCoinTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_token_nieaktywny.png"));
	slotTexSize.x = emptyCoinTexture->getWidth();
	slotTexSize.y = emptyCoinTexture->getHeight();
	lifeManager.coinWidth = slotTexSize.x;
	lifeManager.coinHeight = slotTexSize.y;
	lifeManager.emptyCoinPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(emptyCoinTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);

	auto&& fullCoinTexture = makeTexture(std::string_view("textures/Asset_szkice/Life_token_aktywny.png"));
	slotTexSize.x = fullCoinTexture->getWidth();
	slotTexSize.y = fullCoinTexture->getHeight();
	lifeManager.fullCoinPipeline = renderer.getPipelineManager()->create(
		gfx::pipeline::Pipeline::Desc{
			.vertexShaderPath = "shaders/vertex_default.glsl",
			.fragmentShaderPath = "shaders/fragment_default.glsl",
			.textures = {std::move(fullCoinTexture)},
			.buffers = {defaultUniformBuffer()},
		}
		);

	drawCoins(scene);
}

void LifeManagerSystem::update(Scene& scene) {
	drawCoins(scene);
}