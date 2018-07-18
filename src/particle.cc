#include <iostream>
using std::cout;
using std::endl;

#include "particle.h"
#include "math_utility.h"
#include "shader_utility/pure_color_material.h"
#include "debug_utility/log.h"

ParticleEmitter::ParticleEmitter(
	glm::vec3 direction, 
	float minVelocity, 
	float maxVelocity, 
	float minRadius, 
	float maxRadius):
	direction_(direction),
	min_v_(minVelocity),
	max_v_(maxVelocity),
	min_r_(minRadius),
	max_r_(maxRadius) {
}

void ParticleEmitter::Emit(std::vector<ParticleInfo>& container, glm::vec3 p, int head){
	static float factor = 0.1;
	static int count = 0;
	container[head] = ParticleInfo(
		p,
		0.3f * direction_ + glm::vec3(count % 7 * factor, count % 3 * factor, count % 13 * factor), 
		(min_r_ + max_r_ ) / 2.0
	);
	count ++;
}

Particle::Particle(World* world, Shader* shader, Material* material, Object* object, int amount):
	DeadObject(world, shader, material),
	anchor_(object),
	amount_(amount), 
	particles_(amount), 
	emitter_(glm::vec3(1,0,0)){
	assert(world_);
	vertices_.resize(amount_ * 4);
	if(!shader_)shader_ = new Shader("shader/particle.vert", "shader/particle.frag");
	InitParticles();
}
void Particle::InitParticles(void){
	btVector3 p = anchor_->GetOrigin();
	for(int i = 0; i < amount_; i++){
		emitter_.Emit(particles_,BTVector3ToGLMVec3(p), i);
	}
	head_ = 0;
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
	emitter_.Emit(particles_, BTVector3ToGLMVec3(p), head_ ++);
	if(head_ >= amount_)head_ = 0;
}


