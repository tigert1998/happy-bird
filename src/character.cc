#include <iostream>
using std::cout;
using std::endl;

#include "character.h"
#include "world.h"

float Character::static_pace_(100);

CharacterImpl::CharacterImpl(World* world, const btTransform& transform, btCollisionObject* obj, float speed):Character(speed),world_(world), object_(obj){ }

CharacterImpl::~CharacterImpl(){ }

btCollisionObject* CharacterImpl::GetDelegate(void){
	return object_;
}
void CharacterImpl::Move(bool forward, float step){ // position 2
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	body->setActivationState(ACTIVE_TAG);
	// ResetMove();
	btVector3 v = body->getLinearVelocity() + World::forward * step * Character::static_pace_ * (forward?1:-1);
	if(v[2] > max_speed_)v[2] = max_speed_;
	if(v[2] < -max_speed_)v[2] = -max_speed_;
	body->setLinearVelocity(v);
}
void CharacterImpl::Rotate(bool left, float step){
	// Move by world standard //
	body->setActivationState(ACTIVE_TAG);
	// ResetRotate();
	btVector3 v = body->getLinearVelocity() + World::left * step * Character::static_pace_ * (left?1:-1);
	if (v[0] > max_speed_) v[0] = max_speed_;
	if (v[0] < -max_speed_) v[0] = -max_speed_;
	body->setLinearVelocity(v);
}
void CharacterImpl::Jump(float step){
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	body->setActivationState(ACTIVE_TAG);
	btVector3 velocity = body->getLinearVelocity();
	if(fabs(velocity[1]) > 0.1)return ; // in a jump
	// ignore step
	step = 0.32;
	body->setLinearVelocity(body->getLinearVelocity() + World::up * step * Character::static_pace_);
}
void CharacterImpl::ResetMove(void){ // Error
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	btVector3 v = body->getLinearVelocity();
	body->setActivationState(ACTIVE_TAG);
	body->setLinearVelocity(btVector3(v[0],v[1],0));
	return ;
}
void CharacterImpl::ResetRotate(void){
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	btVector3 v = body->getLinearVelocity();
	body->setActivationState(ACTIVE_TAG);
	body->setLinearVelocity(btVector3(0,v[1],v[2]));
	return ;
}