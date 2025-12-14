#include <PointsCounter.h>
#include <archimedes/Text.h>
#include <archimedes/Font.h>
#include <Config.h>
#include <Defaults.h>
#include <lifes/LifeManager.h>
#include "demon/deals.h"
#include "sound/SFX.h"
#include <systems/EndingSystem.h>
#include <demon/DemonManager.h>

#include "systems/SlotMachine.h"

int PointsCounter::score = 0;
int PointsCounter::threshold = 5000;

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
		SlotMachineSystem::isEnd = true;
		EndingSystem::end(scene, "textures/Asset_final/Ogor_ending.png", "silly_theme.ogg");
		return 0;
	}
	if (ct[0] > 0 and !Deals::no_gurken) {
		SFX::playSFX("nieudane_rozdanie.ogg");
		return 0;
	}
	//Wylosowano piórko + przemiana
	if (ct[1] > 0) {
		if (ct[2] > ct[3] and ct[2] > ct[4] and ct[2] > ct[5] and ct[2] > ct[6]) { ct[2] += ct[1]; } else if (ct[3] > ct[4] and ct[3] > ct[5] and ct[3] > ct[6]) { ct[3] += ct[1]; } else if (ct[4] > ct[5] and ct[4] > ct[6]) { ct[4] += ct[1]; } else if (ct[5] > ct[6]) { ct[5] += ct[1]; } else if (ct[6] > 1) { ct[6] += ct[1]; } else sum += 200 * ct[1]; //same piórka
	}
	//Sumowanie punktów
	sum += 20 * ct[2] + 70 * ct[3] * Deals::better_food + 50 * ct[3] + 120 * ct[4] + 200 * ct[5];
	if (Deals::negative_points!=slots::RewardType::_none) {
		switch (Deals::negative_points) {
			case slots::RewardType::dry:
				sum-=40*ct[2];
				break;
			case slots::RewardType::sachet:
				sum-=100*ct[3]+Deals::better_food*ct[3]*70;
				break;
			case slots::RewardType::feather:
				sum-=240*ct[4];
				break;
			case slots::RewardType::sardines:
				sum-=400*ct[5];
				break;
		}
	}
	lifeManager.updateLifes(ct[6]);
	if (ct[2] > 2 && Deals::negative_points!=slots::RewardType::dry) { sum += 60; }
	if (ct[3] > 2 && Deals::negative_points!=slots::RewardType::sachet) { sum += 150; }
	if (ct[4] > 2 && Deals::negative_points!=slots::RewardType::feather) { sum += 360; }
	if (ct[5] > 2 && Deals::negative_points!=slots::RewardType::sardines) { sum += 600; }
	if (ct[6] > 2) { lifeManager.updateLifes(6); }
	for (auto num : ct) {
		if (num == 3) {
			SFX::playSFX("jackpot.ogg");
		}
	}
	score += sum;
	if (score >= threshold) {
		SlotMachineSystem::isEnd = true;
		DemonManager::isBlocked = true;
		EndingSystem::end(scene, "textures/Asset_final/Good_Ending.png", "happy_ending_theme.ogg");
	}
	return sum;
};

//Wizualny licznik punktów
void PointsCounter::setup(Scene& scene) {
	auto points = scene.newEntity();
	auto&& scoreTransform = points.addComponent(
		scene::components::TransformComponent{
			.position = {windowWidth / 2.f + 5, windowHeight - 202.5f, -0.6f},
			.rotation = {0, 0, 0, 1},
			.scale = {50, 50, 0}
		}
	);
	points.addComponent<ScoreTextFlag>();
}

//Aktualizacja scora na liczniku
void PointsCounter::update(Scene& scene) {
	auto points = scene.entitiesWith<ScoreTextFlag>().front();
	static auto scorePrev = -1;
	static auto originalPos = points.getComponent<scene::components::TransformComponent>().position;

	if (scorePrev != score) {
		scorePrev = score;
		auto buffer = defaultUniformBuffer();
		points.removeComponent<text::TextComponent>();
		auto&& text_comp = points.addComponent(
			text::TextComponent(
				text::convertTo<char32_t>(std::string_view(std::format("{}", score))),
				{buffer},
				"Pixelated Elegance"
			)
		);

		auto&& textT = points.getComponent<scene::components::TransformComponent>();
		auto bottomLeft = text_comp.bottomLeftAdjusted();
		auto topRight = text_comp.topRight();

		auto diff = (topRight - bottomLeft) / 2.f;
		diff.x *= -textT.scale.x;
		diff.y *= -textT.scale.y;

		Logger::debug("diff = {} {}", diff.x, diff.y);

		textT.position = originalPos - diff;
	}
}
