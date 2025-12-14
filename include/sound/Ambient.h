#pragma once
#include <archimedes/Scene.h>

using namespace arch;

struct Ambient {

    static void setAmbient(const std::string& path);

    static void stopAmbient();
};