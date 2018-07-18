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
#include "shader_utility/light_collection.h"
#include "timer.h"

struct ParticleInfo{
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 acceleration;
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
		glm::vec3 a = glm::vec3(0,0,0),
		float radius = 0, 
		Color color = color::White()):
		position(p),
		velocity(v),
		acceleration(a),
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
		// velocity += acceleration;
	}
};

class ParticleEmitter{
	glm::vec3 major_velocity_; // var by acceleration / random
	glm::vec3 acceleration_; // set
	float variance_velocity_;

	float major_radius_; // var by random
	float variance_radius_;

	Color major_color_; // var in range
	Color delta_color_; // range delta
	bool gradual_; // true for gradual change

	float interval_; // one particle per interval
	Timer::TimingId timer_;
 public:
	ParticleEmitter();
	~ParticleEmitter(){ }
	void Emit(std::vector<ParticleInfo>::iterator slot, glm::vec3 p);
	void Update(std::vector<ParticleInfo>::iterator& slot, glm::vec3 p);
};

class Particle: public DeadObject{
	Object* anchor_; // for updating position
	int amount_;
	std::vector<ParticleInfo>::iterator slot_;
	std::vector<ParticleInfo> particles_;
	ParticleEmitter emitter_;
 public:
 	Particle(World* world, Shader* shader, Material* material, Object* object, int amount = 8);
	~Particle(){ }
	void ImportToGraphics();
	void InitParticles(void);
	void Draw(Camera*, const LightCollection*);
};

