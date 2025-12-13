#pragma once
#include "Defaults.h"

namespace Deals {
    extern int sachet;
    extern bool positive_change;
    extern bool negative_change;
    extern int bad_chance;
    extern bool negative_points;
    extern bool glitch;
    extern bool no_gurken;
    extern int hp_chanse;
}

class Deal {
public:
    static void big_sachet();
    static void set_positive_change();
    static void set_negative_change();
    static void set_glitch();
    static void set_no_gurken();
    static void luck_down();
    static void short_on_life();
    static void nolife();
    static void quality();
    static void cheap_bastard();
};