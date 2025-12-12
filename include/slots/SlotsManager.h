#pragma once

namespace slots {

    enum class RewardType {
        sachet,
        catnip,
        dry,
        cucumber,
        sardines,
        feather,
        coin
    };

    class SlotsManager {



    public:
        RewardType generateReward() {
            return RewardType::sachet; // placeholder
        }

    };
}