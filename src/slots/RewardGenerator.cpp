#include <slots/RewardGenerator.h>

namespace slots {
std::string rewardAsString(RewardType type) {
	switch (type) {
		case RewardType::sachet:
			return "sachet";
		case RewardType::catnip:
			return "catnip";
		case RewardType::dry:
			return "dry";
		case RewardType::cucumber:
			return "cucumber";
		case RewardType::sardines:
			return "sardines";
		case RewardType::feather:
			return "feather";
		case RewardType::coin:
			return "coin";
		case RewardType::_none:
			return "none";
	}
	return "";
}

void RewardGenerator::_generateDistribution() {
	_distribution = std::make_shared<std::discrete_distribution<>>(_weights.begin(), _weights.end());
}

RewardType RewardGenerator::generateReward() {
	int index = _distribution->operator()(*_randomGenerator);
	return static_cast<RewardType>(index);
}

RewardGenerator::RewardGenerator() {
	_randomGenerator = std::make_shared<std::mt19937>(_randomDevice());
	_generateDistribution();
}

void RewardGenerator::multiplyProbability(RewardType type, float probability) {
	if (probability < 0) {
		throw std::invalid_argument("probability must be non-negative");
	}
	int index = static_cast<int>(type);
	_weights[index] *= probability;
	float sum = 0;
	for (int i = 0; i < _weights.size(); i++) {
		sum += _weights[i];
	}
	for (auto&& x : _weights) {
		x /= sum;
	}

	_generateDistribution();
}
}