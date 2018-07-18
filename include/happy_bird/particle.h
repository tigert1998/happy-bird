#pragma once

#include <vector>
#include <iostream>
using std::ostream;
using std::endl;
using std::cout;

#include "bullet_common.h" // btCollisionObject
#include "object.h" // DeadObject
#include "color.h" // Color
#include "shader_utility/shader.h"
#include "camera.h"
#include "head.h"
#include "shader_utility/light_collection.h"

struct ParticleInfo{
	glm::vec3 velocity;
	glm::vec3 position;
	float radius;
	Color color;
	ParticleInfo():
		velocity(0, 0, 0),
		position(0, 0, 0),
		radius(1),
		color(1, 1, 1){ }
	ParticleInfo(
		glm::vec3 p,
		glm::vec3 v, 
		float radius = 0, 
		Color color = color::White()):
		position(p),
		velocity(v),
		radius(radius),
		color(color){ }
	ParticleInfo(const ParticleInfo& rhs):
		velocity(rhs.velocity),
		position(rhs.position),
		radius(rhs.radius),
		color(rhs.color) { }
	~ParticleInfo(){ }
	void Update(void){
		position += velocity;
	}
	friend ostream& operator<<(ostream& os, const ParticleInfo& rhs){
		os << "Particle: [v]" << rhs.velocity[0] << ", " << rhs.velocity[1] << ", " << rhs.velocity[2] <<  endl
			<<  "          [p]" << rhs.position[0] << ", " << rhs.position[1] << ", " << rhs.position[2] << endl
			<<  "          [r]" << rhs.radius << endl;
		// system("pause");
		return os;
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
		float minVelocity = 1, 
		float maxVelocity = 2, 
		float minRadius = 7, 
		float maxRadius = 10
	);
	~ParticleEmitter(){ }
	void Emit(std::vector<ParticleInfo>& container, glm::vec3 p, int head);
};

class Particle: public DeadObject{
	Object* anchor_; // for updating position
	int amount_;
	int head_;
	std::vector<ParticleInfo> particles_;
	ParticleEmitter emitter_;
 public:
 	Particle(World* world, Shader* shader, Material* material, Object* object, int amount = 8);
	~Particle(){ }
	void ImportToGraphics();
	void InitParticles(void);
	void Draw(Camera*, const LightCollection*);
};

