#include <iostream>
using std::cout;
using std::endl;

#include "particle.h"
#include "random.h"
#include "math_utility.h"
#include "opengl_common.h"
#include "shader_utility/pure_color_material.h"

ParticleConfig::ParticleConfig(glm::vec3 v, Color color, float interval, int intFlag): 
	major_velocity_(v),
	major_color_(color),
	interval_(interval){
	ParticleFlag flags = static_cast<ParticleFlag>(intFlag) ;
	switch(flags & 0x03){
		case (kGravityParticle) : // Gravity
			acceleration_ = glm::vec3(0, -0.0001, 0);
			break;
		case (kFlameParticle) : // flame
			acceleration_ = glm::vec3(0, 0.0001, 0);
			break;
		default :
			acceleration_ = glm::vec3(0, 0, 0);
	}
	switch(flags & 0x0c){
		case (kLargeParticle):
			major_radius_ = 8;
			variance_radius_ = 4;
			break;
		case (kMediumParticle):
			major_radius_ = 5;
			variance_radius_ = 3;
			break;
		default:
			major_radius_ = 2;
			variance_radius_ = 1;
	}
	switch(flags & 0x10){
		case(kAmbientParticle):
			variance_velocity_ = std::max(std::max(fabs(major_velocity_[0]),fabs(major_velocity_[1])),fabs(major_velocity_[2]));
			break;
		default:
			variance_velocity_ = 0;
	}
	switch(flags & 0x60){
		case(kGradualColorParticle):
			gradual_ = true;
			delta_color_ = glm::vec3(-major_color_[0]/3, -major_color_[1]/3, -major_color_[2]/3);
			break;
		case(kRandomColorParticle):
			gradual_ = false;
			delta_color_ = glm::vec3(-major_color_[0], -major_color_[1], -major_color_[2]);
			break;
		default:
			gradual_ = true;
			delta_color_ = glm::vec3(0, 0, 0);
	}

}

// ParticleEmitter::ParticleEmitter():
// 	major_velocity_(0,0,0.02),
// 	acceleration_(0,0,0),
// 	variance_velocity_(0.02),
// 	major_radius_(5),
// 	variance_radius_(3),
// 	major_color_(color::White()),
// 	delta_color_(-color::White()),
// 	gradual_(false),
// 	interval_(0.07){
// 	timer_ = Timer::New();
// }
ParticleEmitter::ParticleEmitter(ParticleConfig config):config_(config){
	timer_ = Timer::New();
}
void ParticleEmitter::Emit(std::vector<ParticleInfo>::iterator curslot, glm::vec3 p){
	// system("pause"); 
	glm::vec3 v = config_.major_velocity_;
	float min_component = 0.5 + std::min(std::min(fabs(v[0]),fabs(v[1])),fabs(v[2])); // min has largest var, others small var
	float factor = Random::QueryFloatRandom(-config_.variance_velocity_, config_.variance_velocity_);
	v[0] += factor * min_component / (v[0]+0.5);
	factor = Random::QueryFloatRandom(-config_.variance_velocity_, config_.variance_velocity_);
	v[1] += factor * min_component / (v[1]+0.5);
	factor = Random::QueryFloatRandom(-config_.variance_velocity_, config_.variance_velocity_);
	v[2] += factor * min_component / (v[2]+0.5);
	Color color = config_.major_color_;
	factor = Random::QueryFloatRandom(0, 1);
	color[0] += config_.delta_color_[0] * factor;
	if(!config_.gradual_)factor = Random::QueryFloatRandom(0, 1);
	color[1] += config_.delta_color_[1] * factor;
	if(!config_.gradual_)factor = Random::QueryFloatRandom(0, 1);
	color[2] += config_.delta_color_[2] * factor;
	*curslot = ParticleInfo(
		p,
		v,
		config_.acceleration_,
		config_.major_radius_ + Random::QueryFloatRandom(0, config_.variance_radius_),
		color
	);
}
void ParticleEmitter::Update(std::vector<ParticleInfo>::iterator& curslot, glm::vec3 p){
	if(Timer::Query(timer_) >= config_.interval_){
		Emit(curslot, p);
		Timer::Pin(timer_);
	}
	curslot ++;
}

Particle::Particle(
	World* world, 
	Shader* shader, 
	Material* material, 
	Object* object, 
	glm::vec3 offset,
	glm::vec3 velocity, 
	int flags,
	int amount,
	float interval
):DeadObject(world, shader, material),
	anchor_(object),
	offset_(offset),
	amount_(amount), 
	particles_(amount), 
	emitter_(ParticleConfig(velocity, dynamic_cast<PureColorMaterial*>(material)->diffuse(), interval, flags)){
	assert(world_);
	vertices_.resize(amount_ * 4);
	if(!shader_)shader_ = new Shader("shader/particle.vert", "shader/particle.frag");
	InitParticles();
}
void Particle::InitParticles(void){
	btVector3 p = anchor_->GetOrigin();
	glm::vec3 position = BTVector3ToGLMVec3(p) + offset_;
	for(auto slot = particles_.begin(); slot != particles_.end(); slot ++){
		emitter_.Emit(slot,position);
	}
	slot_ = particles_.begin();
}
void Particle::ImportToGraphics(){
	glBindVertexArray(vao_); 
	glBindBuffer(GL_ARRAY_BUFFER, position_vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(btScalar) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(btScalar), (void*) 0);
	glEnableVertexAttribArray(0);
	// point radius
	// glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(btScalar) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
	// glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(btScalar), (void*)0);
	// glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
void Particle::Draw(Camera* camera, const LightCollection* lights) {
	for(int i = 0; i < amount_; i++){
		particles_[i].Update();
		vertices_[4*i + 0] = particles_[i].position[0];
		vertices_[4*i + 1] = particles_[i].position[1];
		vertices_[4*i + 2] = particles_[i].position[2];
		vertices_[4*i + 3] = particles_[i].radius;
	}
	ImportToGraphics();
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", dynamic_cast<PureColorMaterial*>(material_)->diffuse());
	
	btTransform transform;
	transform.setIdentity();

	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->view_matrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->projection_matrix());

	glBindVertexArray(vao_);
	glDrawArrays(GL_POINTS, 0, amount_);
	glBindVertexArray(0);

	btVector3 p = anchor_->GetOrigin();
	glm::vec3 position = BTVector3ToGLMVec3(p) + offset_;
	emitter_.Update(slot_, position);
	if(slot_ >= particles_.end())slot_ = particles_.begin();
}


