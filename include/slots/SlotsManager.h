#pragma once
#include <random>
#include <vector>

namespace slots {

    enum class RewardType {
        sachet, // 50 pkt
        catnip, // dowolny symbol
        dry, // 20 pkt
        cucumber, // killer
        sardines, // 200 pkt
        feather, // 120 pkt
        coin // życie
    };

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
        }
        return "";
    }

    class SlotsManager {
        const std::vector<int> _startingWeightsInt {
            20, //sachet
            10, //catnip
            40, //dry
            1, //cucumber
            5, //sardines
            10, //feather
            10, //coin
        };

        std::vector<float> _weights;

        std::random_device _randomDevice;
        std::shared_ptr<std::mt19937> _randomGenerator;
        std::shared_ptr<std::discrete_distribution<>> _distribution;

        void _initializeWeights() {
            int sum = 0;
            for (auto& w : _startingWeightsInt) {
                sum += w;
            }
            for (int i = 0; i < _startingWeightsInt.size(); i++) {
                _weights.push_back((float)_startingWeightsInt[i] / sum);
            }
        }

        void _generateDistribution() {
            _distribution = std::make_shared<std::discrete_distribution<>>(_weights.begin(), _weights.end());
        }

    public:
        RewardType generateReward() {
            int index = _distribution->operator()(*_randomGenerator);
            return static_cast<RewardType>(index);
        }

        SlotsManager() {
            _initializeWeights();
            _randomGenerator = std::make_shared<std::mt19937>(_randomDevice());
            _generateDistribution();
        }

        void multiplyProbability(RewardType type, float probability) {
            if (probability < 0) {
                throw std::invalid_argument("probability must be non-negative");
            }
            int index = static_cast<int>(type);
            _weights[index] *= probability;
            float sum = 0;
            for (int i = 0; i < _weights.size(); i++) {
                sum += _weights[i];
            }
            for (int i = 0; i < _weights.size(); i++) {
                _weights[i] /= sum;
            }
            _generateDistribution();
        }

    };
}
