#pragma once

#include "light.h"

class Object;

class ParallelLight: virtual public Light {
 protected:
	glm::vec3 direction_;
 public:
	ParallelLight(glm::vec3 direction, glm::vec3 color, float intensity);
	void Attach(std::weak_ptr<Object>);
	LightType type() const; 
	virtual glm::vec3 direction() const;
};