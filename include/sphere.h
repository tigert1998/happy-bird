#pragma once

#include "object.h" // DeadObject
#include "color.h" // Color, color::Red
#include "camera.h"
#include "shader.h"

class World;

class Sphere: public DeadObject{
	float radius_;
 public:
	Sphere() = delete;
	Sphere(World* world, Shader* shader, const btTransform& trans, float radius, Color color = color::Red());
	void Draw(Camera* camera, const Light* light);
	~Sphere();
	// accessor
	inline float radius(void) const;
};