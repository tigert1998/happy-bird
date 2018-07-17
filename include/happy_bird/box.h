#pragma once

#include "opengl_common.h" // glm
#include "bullet_common.h" // btTransform
#include "object.h" // DeadObject
#include "color.h" // Color, color::Yellow
#include "camera.h"
#include "shader_utility/shader.h"

class World;

class Box: public DeadObject{
	glm::vec3 half_extents_;
 public:
	Box() = delete;
	Box(World*, Shader*, Material*, float, const btTransform&, glm::vec3);
	void Draw(Camera* camera, const LightCollection* light_collection);
	~Box(){ }
};
