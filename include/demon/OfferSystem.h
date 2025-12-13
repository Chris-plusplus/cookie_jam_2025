#pragma once

#include <archimedes/Scene.h>

using namespace arch;

namespace demon {
    enum class DemonType {
        happy, distorted, evil
    };

    struct OfferSystem {
        static void setup(Scene& scene);

        static void spawnOfferDialogue(Scene& scene, std::string_view offerText);

        static void clearOfferDialogue(Scene& scene);

        static void update(Scene& scene);
    };
}
