#pragma once
#include <vector>
#include <archimedes/Scene.h>

using namespace arch;

class PointsCounter {
public:
	static int score;
	static int count(Scene& scene, const std::vector<int>&);
	static void setup(Scene& scene);
	static void update(Scene& scene);
};

struct ScoreTextFlag {
	static constexpr bool flagComponent = true;
};
