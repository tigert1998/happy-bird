#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "light.h"

class Lighter{
	std::vector<Light*> lights_;
 public:
 	Lighter() = default;
 	~Lighter(){ }
 	void AddLight(Light*);
	void Feed(const std::string& identifier, Shader *shader) const;
};