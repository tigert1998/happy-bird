#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;

#include "controller_utility/character.h"
#include "world.h"
#include "shader_utility/pure_color_material.h"
#include "particle.h"
#include "plain_box.h"
#include "random.h"
#include "audio.h"

float Character::static_pace_(100);

Character::Character(World* world, shared_ptr<Object> obj): world_(world), object_(obj){ }

Character::~Character(){ }

void Character::set_max_speed(float max_speed) {
	max_speed_ = max_speed;
}

// void Character::Bind(Object* obj){
	// object_ = obj;
// }
std::weak_ptr<Object> Character::GetDelegate(void){
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
	if(fabs(velocity[1]) > 0.3)return ; // in a jump
	// ignore step
	mciSendString((LPCSTR)((std::string("play ") + "resources/sound/ho.wav").c_str()), 0, 0, 0);
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
	if(!object_)return ;
	static Timer::TimingId laser_timer = Timer::New();
	if(Timer::Query(laser_timer) < laser_attack_freq_)return;
	mciSendString((LPCSTR)((std::string("play ") + "resources/sound/laser.wav").c_str()), 0, 0, 0);
	Timer::Pin(laser_timer);
	world_->temp_.PushBack(
		new Particle(
			world_,
			nullptr,
			new PureColorMaterial(laser_attack_color_, laser_attack_color_, 40),
			object_->GetOrigin(),
			glm::vec3(0.25, 0, 0),
			kSmallParticle | kFloatParticle | kLaserParticle,
			10,
			0.05,
			1
		),
		2
	);
	Gain(5);
	Character* caller = this;
	world_->player_collection_ptr_->Query(
		BTVector3ToGLMVec3(object_->GetOrigin()),
		laser_attack_dist_,
		laser_attack_range_,
		[caller](std::weak_ptr<Player> player){
			auto p = player.lock();
			if(p && p->character_ptr().lock().get() != caller){
				p->character_ptr().lock()->Lose(5);
				if(p->character_ptr().lock()->blood() <= 0){
					p->set_is_disabled(true);
				}
			}
		}
	);
}
void Character::BoxAttack(void){
	if(!object_)return ;
	static Timer::TimingId box_timer = Timer::New();
	if(Timer::Query(box_timer) < box_attack_freq_)return;
	mciSendString((LPCSTR)((std::string("play ") + "resources/sound/gun.wav").c_str()), 0, 0, 0);
	Timer::Pin(box_timer);
	btVector3 origin = object_->GetOrigin();
	btTransform transform;
	for(int i = 0; i < 10; i++){
		transform.setIdentity();
		transform.setOrigin( origin + btVector3(Random::QueryFloatRandom(-1,1), Random::QueryFloatRandom(-1,1), Random::QueryFloatRandom(-1,1)) );
		Object* temp = new PlainBox(
				world_, 
				nullptr,
				new PureColorMaterial(box_attack_color_, box_attack_color_, 40),
				transform,
				glm::vec3(0.4, 0.4, 0.4),
				5
			);
		temp->ActivateControl();
		float a = Random::QueryFloatRandom(0, 20),b = Random::QueryFloatRandom(- 30, 30);
		temp->SetVelocity(btVector3(60, a, b));
		// temp->SetVelocity(btVector3(8, Random::QueryFloatRandom(-2,5), Random::QueryFloatRandom(-5, 5)));
		world_->temp_.PushBack(
			temp, 10
		);
	}
	Gain(5);
	Character* caller = this;
	world_->player_collection_ptr_->Query(
		BTVector3ToGLMVec3(object_->GetOrigin()),
		box_attack_dist_,
		box_attack_range_,
		[caller](std::weak_ptr<Player> player){
			auto p = player.lock();
			if(p && p->character_ptr().lock().get() != caller){
				p->character_ptr().lock()->Lose(10);
				if(p->character_ptr().lock()->blood() <= 0){
					p->set_is_disabled(true);
				}
			}
		}
	);
}
void Character::Lose(float amount){
	if (!object_) return;
	blood_ -= amount;
	auto particle = std::make_shared<Particle>(
		world_,
		new Shader("shader/particle.vert", "shader/blood_decr.frag"),
		new PureColorMaterial(color::Red(), color::Red(), 40),
		btVector3(0, 0, 0), // position
		glm::vec3(0, 0.005, 0),
		kMediumParticle | kFloatParticle | kAmbientParticle | kJitterParticle,
		(int)(amount / 2.0),
		1.0f
	);
	particle->Attach(object_, btVector3(0,3,0));
	world_->temp_.PushBack(
		particle, 7
	);
}
void Character::Gain(float amount){
	if(!object_)return ;
	blood_ += amount;
	auto particle = std::make_shared<Particle>(
		world_,
		new Shader("shader/particle.vert", "shader/blood_incr.frag"),
		new PureColorMaterial(color::Green(), color::Green(), 40),
		btVector3(0, 0, 0), // position
		glm::vec3(0, 0.005, 0),
		kMediumParticle | kFloatParticle | kAmbientParticle | kJitterParticle,
		(int)(amount / 2.0),
		2.0f
	);
	particle->Attach(object_, btVector3(0,3,0));
	world_->temp_.PushBack(
		particle, 7
	);
}

