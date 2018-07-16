#pragma once

#include "object.h"
#include "camera.h"
#include "vector_utility.h"

class Sphere: public DeadObject{
	float radius_;

 public:
	Sphere() = delete;
	Sphere(World* world, Shader* shader, const btTransform& trans, float radius, Color color = color::Red());
	void Draw(Camera* camera, const Light* light);
	~Sphere();
	// accessor
	float getRadius(void);

};