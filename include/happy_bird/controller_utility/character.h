#pragma once

#include <memory>

#include "bullet_common.h"
#include "object.h"
#include "color.h"
#include "timer.h"

class World;

class Character {
 protected:
 	static float static_pace_;
 	World* world_;
 	std::shared_ptr<Object> object_;
 	// Config
 	float max_speed_ = 15;
 	float laser_attack_freq_ = 1;
 	float laser_attack_dist_ = 100;
 	float laser_attack_range_ = 7;
 	Color laser_attack_color_ = color::Blue();
 	float box_attack_freq_ = 3;
 	float box_attack_dist_ = 30;
 	float box_attack_range_ = 20; // along depth axis
 	Color box_attack_color_ = color::Red();
 	// runtime data
 	float blood_ = 40;
 public:
 	Character(World*, std::shared_ptr<Object>);
 	void set_max_speed(float max_speed);
 	~Character();
 	// void Bind(Object* object);
 	std::weak_ptr<Object> GetDelegate(void);
 	void Move(bool, float);
 	void Rotate(bool, float);
 	void Jump(float);
 	void ResetMove(void);
 	void ResetRotate(void);
 	void LaserAttack(void);
	void BoxAttack(void);
	void Gain(float);
	void Lose(float);
	float blood(void){return blood_;}
};