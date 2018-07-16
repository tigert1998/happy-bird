#pragma once

#include "cloth.h"

// Attached Object: physicsShape->mesh(mutable)->graphics
class Cloth: public DeadObject{
	float width_;
	float length_;
	int subdivide_;
 public:
	Cloth() = delete;
	Cloth(World* world, Shader* shader, float attachWid, float clothLen, uint32_t subdivide, Head* head, Color color = color::Green());
	~Cloth(){ }
	void Draw(Camera*);
};