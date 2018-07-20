#pragma once

#include <vector>

#include "opengl_common.h" // glm
#include "bullet_common.h" // btTransform
#include "object.h" // DeadObject
#include "color.h" // Color, color::Yellow
#include "shader_utility/shader.h"
#include "camera.h"
#include "world.h"

class World;

class Wall: public Object{
	static float margin_ratio_;
	static float brick_ratio_;
	static std::vector<float> brick_vertices_;
	static std::vector<uint32_t> brick_indices_;
	static std::vector<float> brick_normals_;
	glm::vec3 half_extents_; // the 2 component is abbrecated
	float scaling_; // the brick unit scaling to normalized mesh
 public:
	Wall() = delete;
	// mass is set to 0, dummy input
	Wall(World* world, Shader* shader, Material*, const btTransform& trans, float scaling, glm::vec3 half_extents);
	~Wall(){ }
	// void Draw(Camera* camera, const LightCollection* light_collection);
	void InitMesh();
	void ImportToGraphics(void);
};
