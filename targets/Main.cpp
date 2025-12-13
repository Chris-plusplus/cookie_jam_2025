#include <archimedes/Engine.h>
#include <VulkanVs.h>
#include <Config.h>
#include <slots/RewardGenerator.h>

int main() {
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	arch::Logger::init(arch::LogLevel::debug);

	arch::Unique<arch::Application> application = arch::createUnique<VulkanVs>();

	auto engineConfig = arch::EngineConfig{
		.windowWidth = (int)windowWidth,
		.windowHeight = (int)windowHeight,
		.windowTitle = "VulkanVs",
		.backgroundColor = arch::Color(0, 0, 0, 0),
		.renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK
	};
	//slots::RewardGenerator slotsManager;

	auto engine = arch::Engine(engineConfig, application);
	engine.start();
	// for (int i = 0; i < 100; i++) {
	// 	Logger::info("{}", slots::rewardAsString(slotsManager.generateReward()));
	// }
	// slotsManager.multiplyProbability(slots::RewardType::cucumber, 20);
	// Logger::info("Updated probability for cucumber");
	// for (int i = 0; i < 100; i++) {
	// 	Logger::info("{}", slots::rewardAsString(slotsManager.generateReward()));
	// }
}