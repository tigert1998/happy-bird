#include <iostream>
using std::cout;
using std::endl;

#include "particle.h"
#include "random.h"
#include "math_utility.h"
#include "opengl_common.h"

ParticleEmitter::ParticleEmitter():
	major_velocity_(0,0,0.02),
	acceleration_(0,0,0),
	variance_velocity_(0.02),
	major_radius_(5),
	variance_radius_(3),
	major_color_(color::White()),
	delta_color_(-color::White()),
	gradual_(false),
	interval_(0.07){
	timer_ = kSharedTimer.New();
}
void ParticleEmitter::Emit(std::vector<ParticleInfo>::iterator curslot, glm::vec3 p){
	cout << "Emit one at (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << endl;
	// system("pause"); 
	glm::vec3 v = major_velocity_;
	float min_component = 0.5 + std::min(std::min(v[0],v[1]),v[2]); // min has largest var, others small var
	float factor = Random::QueryFloatRandom(-variance_velocity_, variance_velocity_);
	v[0] += factor * min_component / (v[0]+0.5);
	factor = Random::QueryFloatRandom(-variance_velocity_, variance_velocity_);
	v[1] += factor * min_component / (v[1]+0.5);
	factor = Random::QueryFloatRandom(-variance_velocity_, variance_velocity_);
	v[2] += factor * min_component / (v[2]+0.5);
	Color color = major_color_;
	factor = Random::QueryFloatRandom(0, 1);
	color[0] += delta_color_[0] * factor;
	if(!gradual_)factor = Random::QueryFloatRandom(0, 1);
	color[1] += delta_color_[1] * factor;
	if(!gradual_)factor = Random::QueryFloatRandom(0, 1);
	color[2] += delta_color_[2] * factor;
	*curslot = ParticleInfo(
		p,
		v,
		acceleration_,
		major_radius_ + Random::QueryFloatRandom(0, variance_radius_),
		color
	);
}
void ParticleEmitter::Update(std::vector<ParticleInfo>::iterator& curslot, glm::vec3 p){
	if(kSharedTimer.Query(timer_) >= interval_){
		Emit(curslot, p);
		kSharedTimer.Pin(timer_);
	}
	curslot ++;
}

Particle::Particle(World* world, Shader* shader, Material* material, Object* object, int amount):
	DeadObject(world, shader, material),
	anchor_(object),
	amount_(amount), 
	particles_(amount), 
	emitter_(){
	assert(world_);
	vertices_.resize(amount_ * 4);
	if(!shader_)shader_ = new Shader("shader/particle.vert", "shader/particle.frag");
	InitParticles();
}
void Particle::InitParticles(void){
	btVector3 p = anchor_->GetOrigin();
	for(auto slot = particles_.begin(); slot != particles_.end(); slot ++){
		emitter_.Emit(slot,BTVector3ToGLMVec3(p));
	}
	slot_ = particles_.begin();
}
void Particle::ImportToGraphics(){
	glBindVertexArray(vao_); 
	// point coord
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(btScalar) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, 4*sizeof(btScalar), (void*)0);
	glEnableVertexAttribArray(0);
	// point radius
	// glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(btScalar) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
	// glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(btScalar), (void*)0);
	// glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
void Particle::Draw(Camera* camera, const LightCollection* lights){
	for(int i = 0; i < amount_; i++){
		particles_[i].Update();
		vertices_[4*i + 0] = particles_[i].position[0];
		vertices_[4*i + 1] = particles_[i].position[1];
		vertices_[4*i + 2] = particles_[i].position[2];
		vertices_[4*i + 3] = particles_[i].radius;
	}
	ImportToGraphics();
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", material_->diffuse());
	
	btTransform transform;
	transform.setIdentity();

	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->view_matrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->projection_matrix());

	glBindVertexArray(vao_);
	glDrawArrays(GL_POINTS, 0, amount_);
	glBindVertexArray(0);

	btVector3 p = anchor_->GetOrigin();
	emitter_.Update(slot_, BTVector3ToGLMVec3(p));
	if(slot_ >= particles_.end())slot_ = particles_.begin();
}


