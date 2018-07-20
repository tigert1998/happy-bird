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
	btVector3 v = object_->GetVelocity() + World::forward * step * Character::static_pace_ * (forward?1:-1);
	if(v[2] > max_speed_)v[2] = max_speed_;
	if(v[2] < -max_speed_)v[2] = -max_speed_;
	object_->SetVelocity(v);
}
void CharacterImpl::Rotate(bool left, float step){
	if(!object_)return ;
	btVector3 v = object_->GetVelocity() + World::left * step * Character::static_pace_ * (left?1:-1);
	if(v[0] > max_speed_)v[0] = max_speed_;
	if(v[0] < -max_speed_)v[0] = -max_speed_;
	object_->SetVelocity(v);
}
void CharacterImpl::Jump(float step){
	if(!object_)return ;
	btVector3 velocity = object_->GetVelocity();
	if(fabs(velocity[1]) > 0.1)return ; // in a jump
	// ignore step
	step = 0.32;
	object_->SetVelocity(velocity + World::up * step * Character::static_pace_);
}
void CharacterImpl::ResetMove(void){ // Error
	if(!object_)return ;
	btVector3 v = object_->GetVelocity();
	object_->SetVelocity(btVector3(v[0],v[1],0));
	return ;
}
void CharacterImpl::ResetRotate(void){
	if(!object_)return ;
	btVector3 v = object_->GetVelocity();
	object_->SetVelocity(btVector3(0,v[1],v[2]));
	return ;
}