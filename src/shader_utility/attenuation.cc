#include "shader_utility/attenuation.h"

Attenuation::Attenuation(float range, float constant, float linear, float quadratic) {
	range_ = range;
	constant_ = constant;
	linear_ = linear;
	quadratic_ = quadratic;
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