#pragma once

#include <memory>

#include "bullet_common.h"
#include "object.h"
#include "color.h"
#include "timer.h"

class World;

class Character{
 protected:
 	static float static_pace_;
 	World* world_;
 	Object* object_;
 	// Config
 	float max_speed_ = 15;
 	float laser_attack_freq_ = 1;
 	float laser_attack_dist_ = 10;
 	Color laser_attack_color_ = color::Blue();
 	float box_attack_freq_ = 3;
 	float box_attack_dist_ = 5;
 	float box_attack_range_ = 5; // along depth axis
 	Color box_attack_color_ = color::Red();
 public:
 	Character(World*, Object*);
 	~Character();
 	void Bind(Object* object);
 	Object* GetDelegate(void);
 	void Move(bool, float);
 	void Rotate(bool, float);
 	void Jump(float);
 	void ResetMove(void);
	void ResetRotate(void);
	void LaserAttack(void);
	void BoxAttack(void);
	void Gain(float);
	void Lose(float);
};