#pragma once

#include <archimedes/Scene.h>

using namespace arch;

namespace demon {
    enum class DemonType {
        demon1, demon2, demon3, _none
    };

    enum class OfferType {
        d11,d12,d13,d14,d21,d22,d31,d32,_none
    };

    struct OfferSystem {
        static OfferType offer;

        static void setup(Scene& scene);

        static void spawnOfferDialogue(Scene& scene, std::string_view offerText,OfferType offer);

        static void clearOfferDialogue(Scene& scene);

        static void update(Scene& scene);
    };
}
