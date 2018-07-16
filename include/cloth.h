#pragma once

#include "light.h"

// Attached Object: physicsShape->mesh(mutable)->graphics
class Cloth: public DeadObject{
	float width_;
	float length_;
	int subdivide_;
	btCollisionObject* attached_;
 public:
	Cloth() = delete;
	Cloth(World* world, Shader* shader, float attachWid, float clothLen, uint32_t subdivide, Head* head, Color color = color::Green());
	void ImportToGraphics();
	~Cloth();
	void Draw(Camera* camera, const Light* light);
};