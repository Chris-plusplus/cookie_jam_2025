#include <button/Button.h>
#include <systems/Button.h>
#include <archimedes/Input.h>

void ButtonSystem::setup(Scene& scene, ecs::Entity _entity, float2 topLeft, float2 bottomRight, Ref<gfx::pipeline::Pipeline> onNotHover, Ref<gfx::pipeline::Pipeline> onHover, std::function<void(Scene&, ecs::Entity)> callback) {
	auto entity = Entity(scene, _entity);
	entity.addComponent(
		Button{
			.topLeft = topLeft,
			.bottomRight = bottomRight,
			.onNotHover = onNotHover,
			.onHover = onHover,
			.callback = callback
		}
	);
}

void ButtonSystem::update(Scene& scene) {
	const auto pressed = input::Mouse::left.pressed();
	auto mousePos = input::Mouse::pos();

	for (auto&& [entity, button, transform, mesh] : scene.domain().view<Button, scene::components::TransformComponent, scene::components::MeshComponent>(exclude<Button::InactiveFlag>).all()) {
		auto topLeft = button.topLeft + (float2)transform.position;
		auto bottomRight = button.bottomRight + (float2)transform.position;

		if (mousePos.x >= topLeft.x &&
			mousePos.x < bottomRight.x &&
			mousePos.y > bottomRight.y &&
			mousePos.y <= topLeft.y) {
			if (pressed) {
				button.callback(scene, entity);
			}
			if (button.onHover) {
				mesh.pipeline = button.onHover;
			}
		} else {
			if (button.onNotHover) {
				mesh.pipeline = button.onNotHover;
			}
		}
	}
}