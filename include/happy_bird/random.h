#pragma once

#include <random>
#include <functional>
#include <vector>

class Random{
	static std::random_device device_;
	static std::mt19937 generator_;
	static float gen_max_;
	static float gen_small_;
	static std::uniform_real_distribution<float> large_dis_;
	static std::uniform_real_distribution<float> small_dis_;
 public:
	static int QueryIntRandom(int lo, int hi);
	static float QueryFloatRandom(float lo, float hi);
};