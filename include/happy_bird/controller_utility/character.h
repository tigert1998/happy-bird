#pragma once

#include "bullet_common.h"
#include "object.h"

class World;

class Character {
 protected:
 	static float static_pace_;
 	float max_speed_;
 	Object* object_;
 	World* world_;
 public:
 	Character(World*, Object*, float speed = 15);
 	~Character();
 	void Bind(Object* object);
 	Object* GetDelegate(void);
 	void Move(bool, float);
 	void Rotate(bool, float);
 	void Jump(float);
 	void ResetMove(void);
 	void ResetRotate(void);
 	void LaserAttack(void);
	void BoxAttack(void);
};
