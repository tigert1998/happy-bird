#pragma once

#include "bullet_common.h"

class World;

class Character{
 public:
	World* world_;
	btKinematicCharacterController* controller_;
	btPairCachingGhostObject* ghost_object_;
	Character(World*, const btTransform&, btCollisionShape*);
	~Character();
	// void Move(const btVector3&, float delta);
	void Move(bool forward, float step);
	void Rotate(bool left, float step);;
	void Jump(float step);
	void ResetMove(void);
	btTransform GetTransform(void);
	// void GetTransform(const btTransform& trans);
};
