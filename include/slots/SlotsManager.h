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
        coin // życie
    };

    std::string rewardAsString(RewardType type);

    class SlotsManager {
        const std::vector<int> _startingWeightsInt {
            1, //cucumber
            10, //catnip
            40, //dry
            20, //sachet
            10, //feather
            5, //sardines
            10, //coin
        };

        std::vector<float> _weights;

        std::random_device _randomDevice;
        std::shared_ptr<std::mt19937> _randomGenerator;
        std::shared_ptr<std::discrete_distribution<>> _distribution;

        void _initializeWeights();

        void _generateDistribution();

    public:
        RewardType generateReward();

        SlotsManager();

        void multiplyProbability(RewardType type, float probability);

    };
}
