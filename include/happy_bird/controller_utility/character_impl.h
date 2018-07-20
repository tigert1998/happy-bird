#pragma once

#include "controller_utility/character.h"
#include "world.h"

class CharacterImpl: public Character {
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