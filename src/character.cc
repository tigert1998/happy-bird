#include <iostream>
using std::cout;
using std::endl;

#include "character.h"
#include "world.h"
#include "shader_utility/pure_color_material.h"
#include "particle.h"
#include "plain_box.h"
#include "random.h"

float Character::static_pace_(100);

Character::Character(World* world, Object* obj, float speed):max_speed_(speed),world_(world), object_(obj){ }
Character::~Character(){ }
void Character::Bind(Object* obj){
	object_ = obj;
}
Object* Character::GetDelegate(void){
	return object_;
}
void Character::Move(bool forward, float step){ // position 2
	if(!object_)return ;
	object_->ActivateControl();
	btVector3 v = object_->GetVelocity() + World::forward * step * Character::static_pace_ * (forward?1:-1);
	if(v[2] > max_speed_)v[2] = max_speed_;
	if(v[2] < -max_speed_)v[2] = -max_speed_;
	object_->SetVelocity(v);
}
void Character::Rotate(bool left, float step){
	if(!object_)return ;
	object_->ActivateControl();
	btVector3 v = object_->GetVelocity() + World::left * step * Character::static_pace_ * (left?1:-1);
	if(v[0] > max_speed_)v[0] = max_speed_;
	if(v[0] < -max_speed_)v[0] = -max_speed_;
	object_->SetVelocity(v);
}
void Character::Jump(float step){
	if(!object_)return ;
	object_->ActivateControl();
	btVector3 velocity = object_->GetVelocity();
	if(fabs(velocity[1]) > 0.1)return ; // in a jump
	// ignore step
	step = 0.32;
	object_->SetVelocity(velocity + World::up * step * Character::static_pace_);
}
void Character::ResetMove(void){ // Error
	if(!object_)return ;
	object_->ActivateControl();
	btVector3 v = object_->GetVelocity();
	object_->SetVelocity(btVector3(v[0],v[1],0));
	return ;
}
void Character::ResetRotate(void){
	if(!object_)return ;
	object_->ActivateControl();
	btVector3 v = object_->GetVelocity();
	object_->SetVelocity(btVector3(0,v[1],v[2]));
	return ;
}
void Character::LaserAttack(void){
	world_->temp_.PushBack(
		new Particle(
			world_,
			nullptr,
			new PureColorMaterial(color::Red(), color::Red(), 40),
			object_->GetOrigin(),
			glm::vec3(0.1, 0, 0),
			kSmallParticle | kFloatParticle | kLaserParticle,
			10,
			0.02,
			1
		),
		2
	);
}
void Character::BoxAttack(void){
	btVector3 origin = object_->GetOrigin();
	btTransform transform;
	for(int i = 0; i < 10; i++){
		transform.setIdentity();
		transform.setOrigin( origin + btVector3(Random::QueryFloatRandom(-1,1), Random::QueryFloatRandom(-1,1), Random::QueryFloatRandom(-1,1)) );
		Object* temp = new PlainBox(
				world_, 
				nullptr,
				new PureColorMaterial(color::Red(), color::Red(), 40),
				transform,
				glm::vec3(0.4, 0.4, 0.4),
				5
			);
		temp->ActivateControl();
		float a = Random::QueryFloatRandom(0, 20),b = Random::QueryFloatRandom(- 30, 30);
		cout << "New bullet: " << a << ", " << b << endl;
		temp->SetVelocity(btVector3(60, a, b));
		// temp->SetVelocity(btVector3(8, Random::QueryFloatRandom(-2,5), Random::QueryFloatRandom(-5, 5)));
		world_->temp_.PushBack(
			temp, 10
		);
	}

}