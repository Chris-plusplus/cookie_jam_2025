#include <systems/MultilineText.h>
#include <Defaults.h>

void MultilineTextSystem::setup(Scene& scene, ecs::Entity _parent, const std::u32string& text, font::Face& font, const std::vector<std::string>& shaders) {
	remove(scene, _parent);

	Logger::debug("xdxdxdxd {}", text::convertTo<char>(std::u32string_view(text)));

	auto lines = text | std::views::split('\n') | std::views::transform([](auto&& subview) {
		return std::u32string_view(subview.begin(), subview.end());
	}) | std::ranges::to<std::vector>();

	auto parent = Entity(scene, _parent);
	for (auto&& [i, line] : lines | std::views::enumerate) {
		auto newLine = parent.addChild();

		auto&& parentT = parent.getComponent<scene::components::TransformComponent>();

		auto&& transform = newLine.addComponent<scene::components::TransformComponent>(parentT);
		transform.position.y -= font.metrics().lineHeight * parentT.scale.y * i;

		Logger::debug("{}, {}, {}", parentT.position.x, parentT.position.y, parentT.position.z);

		auto&& text = newLine.addComponent(
			text::TextComponent(
				std::u32string(line.begin(), line.end()),
				{defaultUniformBuffer()},
				font,
				shaders[i % shaders.size()]
			)
		);
	}
}

void MultilineTextSystem::remove(Scene& scene, ecs::Entity _parent) {
	auto parent = Entity(scene, _parent);
	std::vector<ecs::Entity> toRemove;
	auto ccount = parent.childrenCount();
	toRemove.reserve(ccount);

	if (parent.childrenCount() != 0) {
		for (auto&& child : parent.children()) {
			if (child.hasComponent<text::TextComponent>()) {
				toRemove.push_back(child.handle());
			}
		}

		for (auto&& toRem : toRemove) {
			scene.removeEntity(toRem);
		}
	}
}