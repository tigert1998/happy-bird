#pragma once

#include "object.h" // DeadObject
#include "color.h" // Color, color::Red
#include "camera.h"
#include "shader_utility/shader.h"

class World;

class Sphere: public Object{
	float radius_;
 public:
	Sphere() = delete;
	Sphere(World* world, Shader* shader, Material* material, const btTransform& trans, float radius);
	~Sphere();
	// accessor
	inline float radius(void) const;
	void ImportToGraphics(void);
};