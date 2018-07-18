#include <vector>
#include <iostream>
#include <cmath>

#include "shader_utility/attenuation.h"

Attenuation::Attenuation(float range) {
	static std::vector<float> lookup_ {
		7.0f, 1.0f, 0.7f, 1.8f,
		13.0f, 1.0f, 0.35f, 0.44f,
		20.0f, 1.0f, 0.22f, 0.2f,
		32.0f, 1.0f, 0.14f, 0.07f,
		50.0f, 1.0f, 0.09f, 0.032f,
		65.0f, 1.0f, 0.07f, 0.017f,
		100.0f, 1.0f, 0.045f, 0.0075f,
		160.0f, 1.0f, 0.027f, 0.0028f,
		200.0f, 1.0f, 0.022f, 0.0019f,
		325.0f, 1.0f, 0.014f, 0.0007f,
		600.0f, 1.0f, 0.007f, 0.0002f,
		3250.0f, 1.0f, 0.0014f, 0.000007f
	};
	range_ = range;
	int level;
	for(level = 0; lookup_[level * 4] < range && level < lookup_.size() / 4; level ++);
	constant_ = 1.0;
	if (level == 0) {
		constant_ = lookup_[1];
		linear_ = lookup_[2];
		quadratic_ = lookup_[3];
	}
	else if(level == lookup_.size() / 4) {
		constant_ = lookup_[(level - 1) * 4 + 1];
		linear_ = lookup_[(level - 1) * 4 + 2];
		quadratic_ = lookup_[(level - 1) * 4 + 3];
	}
	else{
		constant_ = lookup_[level * 4 + 1];
		linear_ = 
			lookup_[(level - 1) * 4 + 2] + 
			(range-lookup_[(level - 1) * 4]) / (lookup_[level * 4]-lookup_[(level - 1) * 4]) * (lookup_[level * 4 + 2]-lookup_[(level - 1) * 4 + 2]);
		quadratic_ = 
			lookup_[(level-1)*4 + 2] + 
			pow(range-lookup_[(level - 1) * 4], 2) / pow(lookup_[level * 4]-lookup_[(level - 1) * 4], 2) * (lookup_[level * 4 + 3] - lookup_[(level - 1) * 4 + 3]);
	}
}

float Attenuation::range() const {
	return range_;
}
	
float Attenuation::constant() const {
	return constant_;
}

float Attenuation::linear() const {
	return linear_;
}

float Attenuation::quadratic() const {
	return quadratic_;
}