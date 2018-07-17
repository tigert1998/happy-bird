#pragma once

#include <vector>

#include "bullet_common.h" // btCollisionObject
#include "object.h" // DeadObject
#include "color.h" // Color
#include "shader.h"
#include "camera.h"
#include "head.h"
#include "lighter.h"

struct ParticleInfo{
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 acceleration;
	float radius;
	Color color;
	ParticleInfo():
		velocity(0, 0, 0),
		position(0, 0, 0),
		acceleration(0, 0, 0),
		radius(1),
		color(1, 1, 1){ }
	ParticleInfo(
		glm::vec3 v, 
		glm::vec3 a, 
		float radius = 0, 
		Color color = color::White()):
		velocity(v),
		position(0, 0, 0),
		acceleration(a),
		radius(1),
		color(color){ }
	ParticleInfo(const ParticleInfo& rhs):
		velocity(rhs.velocity),
		position(rhs.position),
		acceleration(rhs.acceleration),
		radius(rhs.radius),
		color(rhs.color) { }
	~ParticleInfo(){ }
	void Update(void){
		position += velocity;
		velocity += acceleration;
	}
};

class ParticleEmitter{
	glm::vec3 direction_;
	float min_v_;
	float max_v_;
	float min_r_;
	float max_r_;
 public:
	ParticleEmitter(
		glm::vec3 direction, 
		float minVelocity = 0, 
		float maxVelocity = 2, 
		float minRadius = 1, 
		float maxRadius = 3
	);
	~ParticleEmitter(){ }
	void Emit(std::vector<ParticleInfo>& container, int head);
};

class Particle: public DeadObject{
	Object* anchor_; // for updating position
	int amount_;
	int head_;
	std::vector<ParticleInfo> particles_;
	ParticleEmitter emitter_;
 public:
 	Particle(World* world, Shader* shader, Object* object, int amount = 20);
	~Particle(){ }
	void ImportToGraphics();
	void InitParticles(void);
	void Draw(Camera*, const Lighter*);
};

