#include <systems/StaticText.h>
#include <Defaults.h>

namespace vs {

void StaticTextSystem::setup(Scene& scene, const std::u32string& text, font::Face& font, f32 fontSize, float3 pos, f32 angle) {
	auto e = scene.newEntity();
	e.addComponent(
		scene::components::TransformComponent{
			.position = pos,
			.rotation = glm::angleAxis(angle, zAxis()) * glm::quat(0, 0, 0, 1),
			.scale = {fontSize, fontSize, 0}
		}
	);
	e.addComponent(
		text::TextComponent(text, {defaultUniformBuffer()}, font)
	);
}

}