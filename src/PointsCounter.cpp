#include <PointsCounter.h>
#include <archimedes/Text.h>
#include <archimedes/Font.h>
#include <Config.h>
#include <Defaults.h>
#include <lifes/LifeManager.h>
#include "demon/deals.h"
#include "sound/SFX.h"

int PointsCounter::score = 0;

int PointsCounter::count(Scene& scene, const std::vector<int>& wyniki) {
	int sum = 0;
	auto&& manager = scene.domain().view<LifeManager>().front();
	auto&& lifeManager = scene.domain().getComponent<LifeManager>(manager);
	//Zliczanie wyników losowania
	int ct[] = {0, 0, 0, 0, 0, 0, 0};
	for (int i : wyniki) {
		ct[i] += 1;
	}
	//Wylosowano ogórki
	if (ct[0] > 2) {
		//TODO GAME OVER
		return 0;
	}
	if (ct[0] > 0 and !Deals::no_gurken) {
		SFX::playSFX(scene, "nieudane_rozdanie.ogg");
		return 0;
	}
	//Wylosowano piórko + przemiana
	if (ct[1] > 0) {
		if (ct[2] > ct[3] and ct[2] > ct[4] and ct[2] > ct[5] and ct[2] > ct[6]) { ct[2] += ct[1]; } else if (ct[3] > ct[4] and ct[3] > ct[5] and ct[3] > ct[6]) { ct[3] += ct[1]; } else if (ct[4] > ct[5] and ct[4] > ct[6]) { ct[4] += ct[1]; } else if (ct[5] > ct[6]) { ct[5] += ct[1]; } else if (ct[6] > 1) { ct[6] += ct[1]; } else sum += 200 * ct[1]; //same piórka
	}
	//Sumowanie punktów
	sum += 20 * ct[2] + 30 * ct[2] * Deals::better_food + 50 * ct[3] + 120 * ct[4] + 200 * ct[5];
	lifeManager.updateLifes(ct[6]);
	if (ct[2] > 2) { sum += 60; }
	if (ct[3] > 2) { sum += 150; }
	if (ct[4] > 2) { sum += 360; }
	if (ct[5] > 2) { sum += 600; }
	if (ct[6] > 2) { lifeManager.updateLifes(6); }
	for (auto num : ct) {
		if (num == 3) {
			SFX::playSFX(scene, "jackpot.ogg");
		}
	}
	score += sum;
	return sum;
};

//Wizualny licznik punktów
void PointsCounter::setup(Scene& scene) {
	auto points = scene.newEntity();
	auto&& scoreTransform = points.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2.f - 10, windowHeight - 225.f, -0.6f},
			.rotation = {0, 0, 0, 1},
			.scale = {50, 50, 0}
		}
	);
	points.addComponent<ScoreTextFlag>();
}

//Aktualizacja scora na liczniku
void PointsCounter::update(Scene& scene) {
	auto buffer = defaultUniformBuffer();
	auto points = scene.entitiesWith<ScoreTextFlag>().front();
	points.removeComponent<text::TextComponent>();
	auto&& text_comp= points.addComponent(
		text::TextComponent(
			text::convertTo<char32_t>(std::string_view(std::format("{}", score))),
			{buffer},
			"Pixelated Elegance"
		)
	);
}
