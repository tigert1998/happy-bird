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
	bool shrink;
	ParticleInfo():
		velocity(0, 0, 0),
		position(0, 0, 0),
		radius(1),
		color(1, 1, 1),
		shrink(false){ }
	ParticleInfo(
		glm::vec3 p,
		glm::vec3 v, 
		glm::vec3 a = glm::vec3(0,0,0),
		float radius = 0, 
		Color color = color::White(),
		bool shrink = false):
		position(p),
		velocity(v),
		acceleration(a),
		radius(radius),
		color(color),
		shrink(shrink){ }
	ParticleInfo(const ParticleInfo& rhs):
		velocity(rhs.velocity),
		position(rhs.position),
		radius(rhs.radius),
		color(rhs.color) { }
	~ParticleInfo(){ }
	void Update(float delta){
		position += velocity * float(Timer::kFrameElapsed * 500);
		velocity += acceleration * float(Timer::kFrameElapsed * 500);
		if(shrink)radius -= radius * float(Timer::kFrameElapsed);
	}
};
enum ParticleFlag{
	// acceleration
	kDefaultParticle = 0x00,
	kGravityParticle = 0x01,
	kFlameParticle = 0x02,
	kFloatParticle = 0x03, // 00 for float
	// radius
	kSmallParticle = 0x04,
	kLargeParticle = 0x08,
	kMediumParticle = 0x0c, // 00 for small
	// velocity var
	kAmbientParticle = 0x00,
	kLaserParticle = 0x10, // 00 for ambient
	// color var
	kGradualColorParticle = 0x20,
	kMonoColorParticle = 0x40,
	kRandomColorParticle = 0x60, // 00 for mono
	// origin var
	kJitterParticle = 0x80,
	// Special effect
	kMockFlame = 0x100
} ;
struct ParticleConfig{
	// explicit assigned
	glm::vec3 major_velocity; // var by acceleration / random
	// kGravityParticle, kFlameParticle, kFloatParticle
	glm::vec3 acceleration; // set
	// kAmbientParticle, kLaserParticle
	float variance_velocity;

	// kLargeParticle, kSmallParticle, kMediumParticle
	float major_radius; // var by random
	float variance_radius;

	// assigned by material
	Color major_color; // var in range
	// kGradualColorParticle, kMonoColorParticle, kRandomColorParticle
	Color delta_color; // range delta
	bool gradual; // true for gradual change
	// external option
	bool jitter;
	bool mock_flame;

	ParticleConfig(glm::vec3 v, Color color, int flags = 0); 
};


class ParticleEmitter{
	ParticleConfig config_;
 public:
	ParticleEmitter();
	ParticleEmitter(ParticleConfig config);
	~ParticleEmitter(){ }
	void Emit(std::vector<ParticleInfo>::iterator slot, glm::vec3 p);
	void Update(std::vector<ParticleInfo>::iterator& slot, glm::vec3 p);
};

class Particle: public Object{
	btVector3 position_;
	int amount_;
	std::vector<ParticleInfo>::iterator slot_;
	std::vector<ParticleInfo> particles_;
	ParticleEmitter emitter_;
	float interval_; // one particle per interval
	float duration_;	
	Timer::TimingId interval_timer_;
	Timer::TimingId duration_timer_;
 public:
 	Particle(
 		World* world, 
 		Shader* shader, 
 		Material* material, 
 		const btVector3& position,
 		glm::vec3 velocity, 
 		int flags,
 		int amount = 20,
 		float interval = 0.07,
 		float duration = 0 // 0 for no limit
 	);
	~Particle(){ }
	btVector3 GetOrigin(void);
	void ImportToGraphics();
	void InitMesh(void);
	void Draw(Camera*, const LightCollection*);
};

