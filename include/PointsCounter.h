#pragma once
#include <vector>
#include <archimedes/Scene.h>

using namespace arch;

class PointsCounter {
public:
    int score=0;
    int count(const std::vector<int>&);
    static void setup(Scene& scene);
    static void update(Scene& scene);
};
struct ScoreTextFlag {};
