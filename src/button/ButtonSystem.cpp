#include <button/Button.h>
#include <systems/Button.h>
#include <archimedes/Input.h>

void ButtonSystem::setup(Scene& scene, ecs::Entity _entity, float2 topLeft, float2 bottomRight, std::function<void(Scene&, ecs::Entity)> callback) {
	auto entity = Entity(scene, _entity);
	entity.addComponent(
		Button{
			.topLeft = topLeft,
			.bottomRight = bottomRight,
			.callback = callback
		}
	);
}

void ButtonSystem::update(Scene& scene) {
	if (input::Mouse::left.pressed()) {
		auto mousePos = input::Mouse::pos();
		for (auto&& [entity, button, transform] : scene.domain().view<Button, scene::components::TransformComponent>(exclude<Button::InactiveFlag>).all()) {
			auto topLeft = button.topLeft + (float2)transform.position;
			auto bottomRight = button.bottomRight + (float2)transform.position;

			if (mousePos.x >= topLeft.x &&
				mousePos.x < bottomRight.x &&
				mousePos.y > bottomRight.y &&
				mousePos.y <= topLeft.y) {
				button.callback(scene, entity);
			}
		}
	}
}