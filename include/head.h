#pragma once

#include "head.h"

class Character;
class Camera;

class Head: public LivingObject{
	float radius_;
 public:
	btRigidBody* delegate_;
 	Head() = delete;
	Head(World*, Shader*,	const btTransform&, float, Color color = color::Red());
	~Head(){ }
	void Draw(Camera*);
	inline float getRadius(void) const {return radius_;}
};

