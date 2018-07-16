#pragma once

#include "opengl_common.h" // glm
#include "bullet_common.h" // btTransform
#include "object.h" // DeadObject
#include "color.h" // Color, color::Yellow
#include "shader.h"
#include "camera.h"

class World;

class Box: public DeadObject{
	glm::vec3 half_extents_;
 public:
	Box() = delete;
	Box(World* world, Shader* shader, const btTransform& trans, glm::vec3 half_extents, Color color = color::Yellow());
	void Draw(Camera* camera);
	~Box(){ }
};
