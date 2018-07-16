#pragma once

#include "object.h"

class Sphere: public DeadObject{
	float radius_;
 public:
	Sphere() = delete;
	Sphere(World* world, Shader* shader, const btTransform& trans, float radius, Color color = color::Red());
	void Draw(Camera* camera);
	~Sphere(){ }
	// accessor
	inline float getRadius(void){return radius_;}
};