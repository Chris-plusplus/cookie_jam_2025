#include <archimedes/Engine.h>
#include <VulkanVs.h>
#include <Config.h>

int main() {
	arch::Logger::init(arch::LogLevel::debug);

	arch::Unique<arch::Application> application = arch::createUnique<VulkanVs>();

	auto engineConfig = arch::EngineConfig{
		.windowWidth = (int)windowWidth,
		.windowHeight = (int)windowHeight,
		.windowTitle = "VulkanVs",
		.backgroundColor = arch::Color(0, 0, 0, 0),
		.renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK
	};

	auto engine = arch::Engine(engineConfig, application);
	engine.start();
}