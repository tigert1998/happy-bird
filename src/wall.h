#pragma once

#include <vector>

#include "opengl_common.h" // glm
#include "bullet_common.h" // btTransform
#include "object.h" // DeadObject
#include "color.h" // Color, color::Yellow
#include "shader.h"
#include "camera.h"
#include "world.h"

class World;

class Wall: public DeadObject{
	static float margin_ratio_;
	static float brick_ratio_;
	static std::vector<float> brick_vertices_;
	glm::vec3 half_extents_; // the 2 component is abbrecated
	float scaling_; // the brick unit scaling to normalized mesh
 public:
	Wall() = delete;
	// mass is set to 0, dummy input
	Wall(World* world, Shader* shader, const btTransform& trans, float scaling, glm::vec3 half_extents, Color color = color::Yellow());
	~Wall(){ }
	void InitBricks(const btTransform& transform);
};
