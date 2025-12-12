#include <systems/Kill.h>
#include <components/Kill.h>
#include <ranges>


void KillSystem::update(Scene& scene) {
	// killing from behind -> view iterators don't get invalidated
	for (auto&& entity : scene.domain().view<Kill>() | std::views::reverse) {
		scene.removeEntity(entity);
	}
}

