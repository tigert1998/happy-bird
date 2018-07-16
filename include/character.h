#pragma once

#include "bullet_common.h"

class World;

class Character{
 public:
	World* world_;
	btKinematicCharacterController* controller_;
	btPairCachingGhostObject* ghost_object_;
	Character(World*, btCollisionShape*);
	~Character();
	void Move(const btVector3&, float delta);
	void Jump(float step);
};
