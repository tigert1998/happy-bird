#include <iostream>
using std::cout;
using std::endl;

#include "character.h"
#include "world.h"

float Character::static_pace_(100);

CharacterImpl::CharacterImpl(World* world, Object* obj, float speed):Character(speed),world_(world), object_(obj){ }
CharacterImpl::~CharacterImpl(){ }
void CharacterImpl::Bind(Object* obj){
	object_ = obj;
}
Object* CharacterImpl::GetDelegate(void){
	return object_;
}
void CharacterImpl::Move(bool forward, float step){ // position 2
	if(!object_)return ;
	// btTransform trans = object_->getWorldTransform();
	// btVector3 forwardDir = trans.getBasis()[2].normalize();
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_->bt_object_);
	body->setActivationState(ACTIVE_TAG);
	// ResetMove();
	btVector3 v = body->getLinearVelocity() + World::forward * step * Character::static_pace_ * (forward?1:-1);
	if(v[2] > max_speed_)v[2] = max_speed_;
	if(v[2] < -max_speed_)v[2] = -max_speed_;
	body->setLinearVelocity(v);
}
void CharacterImpl::Rotate(bool left, float step){
	if(!object_)return ;
	// Try rotating //
	// btMatrix3x3 orn = object_->getWorldTransform().getBasis();
	// orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),0.01 * (left?-1:1) ));
	// step = 10;
	// Try move left-right //
	// object_->getWorldTransform().setBasis(orn);
	// btTransform trans = object_->getWorldTransform();
	// btVector3 leftDir = trans.getBasis()[0].normalize();
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_->bt_object_);
	// Move by world standard //
	body->setActivationState(ACTIVE_TAG);
	// ResetRotate();
	btVector3 v = body->getLinearVelocity() + World::left * step * Character::static_pace_ * (left?1:-1);
	if(v[0] > max_speed_)v[0] = max_speed_;
	if(v[0] < -max_speed_)v[0] = -max_speed_;
	body->setLinearVelocity(v);
	// body->setLinearVelocity(body->getLinearVelocity() + World::left * step * Character::static_pace_ * (left?1:-1));
}
void CharacterImpl::Jump(float step){
	if(!object_)return ;
	// btTransform trans = object_->getWorldTransform();
	// btVector3 upDir = trans.getBasis()[1].normalize();
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_->bt_object_);
	body->setActivationState(ACTIVE_TAG);
	btVector3 velocity = body->getLinearVelocity();
	if(fabs(velocity[1]) > 0.1)return ; // in a jump
	// ignore step
	step = 0.32;
	body->setLinearVelocity(body->getLinearVelocity() + World::up * step * Character::static_pace_);
}
void CharacterImpl::ResetMove(void){ // Error
	if(!object_)return ;
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_->bt_object_);
	btVector3 v = body->getLinearVelocity();
	body->setActivationState(ACTIVE_TAG);
	body->setLinearVelocity(btVector3(v[0],v[1],0));
	return ;
}
void CharacterImpl::ResetRotate(void){
	if(!object_)return ;
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_->bt_object_);
	btVector3 v = body->getLinearVelocity();
	body->setActivationState(ACTIVE_TAG);
	body->setLinearVelocity(btVector3(0,v[1],v[2]));
	return ;
}