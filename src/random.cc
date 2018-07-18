#include <iostream>
using std::cout;
using std::endl;

#include "random.h"

std::random_device Random::device_;
std::mt19937 Random::generator_(device_());
float Random::gen_max_(10000000);
float Random::gen_small_(100);
std::uniform_real_distribution<float> Random::small_dis_(0, gen_small_);
std::uniform_real_distribution<float> Random::large_dis_(0, gen_max_);
int Random::QueryIntRandom(int lo, int hi){
	if(hi - lo > gen_max_){
		std::uniform_int_distribution<int> tmp(lo, hi);
		return tmp(generator_);
	}
	else if(hi - lo <= gen_small_){
		return static_cast<int>(small_dis_(generator_) * (hi-lo) / gen_max_ + lo);
	}
	else{
		return static_cast<int>(large_dis_(generator_) * (hi-lo) / gen_max_ + lo);
	}
}
float Random::QueryFloatRandom(float lo, float hi){
	if(hi - lo > gen_max_){
		std::uniform_real_distribution<float> tmp(lo, hi);
		return tmp(generator_);
	}
	else if(hi - lo <= gen_small_){
		return static_cast<float>(small_dis_(generator_) * (hi-lo) / gen_small_ + lo);
	}
	else{
		return static_cast<float>(large_dis_(generator_) * (hi-lo) / gen_max_ + lo);
	}
}