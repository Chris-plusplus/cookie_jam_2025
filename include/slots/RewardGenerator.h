
#pragma once
#include <random>
#include <vector>

namespace slots {

enum class RewardType {
	cucumber, // killer
	catnip, // dowolny symbol
	dry, // 20 pkt
	sachet, // 50 pkt
	feather, // 120 pkt
	sardines, // 200 pkt
	coin, // życie
	_count,
	_none
};

std::string rewardAsString(RewardType type);

class RewardGenerator {
	std::vector<float> _weights{
		.01, //cucumber
		.1, //catnip
		.4, //dry
		.2, //sachet
		.1, //feather
		.09, //sardines
		.1, //coin
	};
	//std::vector<float> _weights{
	//	.1, //cucumber
	//	.0, //catnip
	//	.4, //dry
	//	.0, //sachet
	//	.0, //feather
	//	.0, //sardines
	//	.0, //coin
	//};

	std::random_device _randomDevice;
	std::shared_ptr<std::mt19937> _randomGenerator;
	std::shared_ptr<std::discrete_distribution<>> _distribution;

	void _generateDistribution();

public:
	RewardType generateReward();

	RewardGenerator();

	void multiplyProbability(RewardType type, float probability);

};
}
