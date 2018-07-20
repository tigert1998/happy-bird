#pragma once

#include "light.h"

class ParallelLight: virtual public Light {
private:
	glm::vec3 direction_;
	
public:
	ParallelLight(glm::vec3 direction, glm::vec3 color, float intensity);
	LightType type() const; 
	glm::vec3 direction() const;
};