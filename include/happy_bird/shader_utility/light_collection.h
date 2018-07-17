#pragma once

#include "light.h"

#include <glm/glm.hpp>

#include <vector>

class LightCollection {
private:
	glm::vec3 ambient_;
	std::vector<const Light*> light_ptrs_;

public:
	LightCollection(glm::vec3 ambient);
 	void PushBack(const Light* light);
 	const Light* operator[](int i) const;
 	int Size() const;
 	glm::vec3 ambient() const;
};