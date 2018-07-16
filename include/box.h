#pragma once

#include "object.h"

class Box: public DeadObject{
	glm::vec3 half_extents_;
 public:
	Box() = delete;
	Box(World* world, Shader* shader, const btTransform& trans, glm::vec3 half_extents, Color color = color::Yellow());
	void Draw(Camera* camera, const Light* light);
	~Box(){ }
};
