#pragma once

#include <chrono>


// Component which stores spawn time of explosion text
struct BoomTextTime {
	decltype(std::chrono::high_resolution_clock::now()) value;
};

