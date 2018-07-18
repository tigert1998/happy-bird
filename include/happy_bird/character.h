#pragma once

#include "bullet_common.h"

class World;
class LivingObject;

class Character{
	friend LivingObject;
 protected:
 	static float static_pace_;
 	float max_speed_;
 public:
 	Character(float speed = 15):max_speed_(speed){ };
 	virtual ~Character(){ }
 	virtual btCollisionObject* GetDelegate(void) = 0;
 	virtual void Move(bool, float) = 0;
 	virtual void Rotate(bool, float) = 0;
 	virtual void Jump(float) = 0;
 	virtual void ResetMove(void) = 0;
 	virtual void ResetRotate(void) = 0;
};
class CharacterBullet: public Character{
 protected:
	World* world_;
	btKinematicCharacterController* controller_;
	btPairCachingGhostObject* ghost_object_;
	btCollisionObject* delegate_;
	void UpdateDelegate(void);
 public:
	CharacterBullet(World*, const btTransform&, btCollisionObject*);
	~CharacterBullet();
	// void Move(const btVector3&, float delta);
	btCollisionObject* GetDelegate(void);
	void Move(bool forward, float step);
	void Rotate(bool left, float step);
	void Jump(float step);
	void ResetMove(void);
	void ResetRotate(void);
};
class CharacterImpl: public Character{
 protected:
 	World* world_;
 	btCollisionObject* object_;
 public:
 	CharacterImpl(World*, const btTransform&, btCollisionObject*);
 	~CharacterImpl();
 	btCollisionObject* GetDelegate(void);
 	void Move(bool, float);
 	void Rotate(bool, float);
 	void Jump(float);
 	void ResetMove(void);
	void ResetRotate(void);
};