#pragma once

#include "bullet_common.h"
#include "object.h"

class World;

class Character{
 protected:
 	static float static_pace_;
 	float max_speed_;
 public:
 	Character(float speed):max_speed_(speed){ };
 	virtual ~Character(){ }
 	virtual void Bind(Object*) = 0;
 	virtual Object* GetDelegate(void) = 0;
 	virtual void Move(bool, float) = 0;
 	virtual void Rotate(bool, float) = 0;
 	virtual void Jump(float) = 0;
 	virtual void ResetMove(void) = 0;
 	virtual void ResetRotate(void) = 0;
};
class CharacterImpl: public Character{
 protected:
 	World* world_;
 	Object* object_;
 public:
 	CharacterImpl(World*, Object*, float speed = 15);
 	~CharacterImpl();
 	void Bind(Object* object);
 	Object* GetDelegate(void);
 	void Move(bool, float);
 	void Rotate(bool, float);
 	void Jump(float);
 	void ResetMove(void);
	void ResetRotate(void);
};