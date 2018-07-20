#pragma once

#include "bullet_common.h"
#include "object.h"

class World;

class Character{
 protected:
 	static float static_pace_;
 	float max_speed_;
 public:
 	Character(float speed);
 	virtual ~Character();
 	virtual void Bind(Object*) = 0;
 	virtual Object* GetDelegate(void) = 0;
 	virtual void Move(bool, float) = 0;
 	virtual void Rotate(bool, float) = 0;
 	virtual void Jump(float) = 0;
 	virtual void ResetMove(void) = 0;
 	virtual void ResetRotate(void) = 0;
};

