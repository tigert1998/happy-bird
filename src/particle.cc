#include <iostream>
using std::cout;
using std::endl;

#include "particle.h"

ParticleEmitter::ParticleEmitter(
	glm::vec3 direction, 
	float minVelocity, 
	float maxVelocity, 
	float minRadius, 
	float maxRadius
):
	direction_(direction),
	min_v_(minVelocity),
	max_v_(maxVelocity),
	min_r_(minRadius),
	max_r_(maxRadius) {
}
void ParticleEmitter::Emit(std::vector<ParticleInfo>& container, int head){
	container[head] = ParticleInfo(
		direction_ * ((min_v_ + max_v_) / 2.0f), 
		glm::vec3(0, 0, 0),
		(min_r_ + max_r_ ) / 2.0
	);
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
	for(int i = 0; i < amount_; i++){
		emitter_.Emit(particles_, i);
	}
	head_ = 0;
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
		vertices_[3*i + 0] = particles_[i].position[0];
		vertices_[3*i + 1] = particles_[i].position[1];
		vertices_[3*i + 2] = particles_[i].position[2];
		vertices_[3*i + 3] = particles_[i].radius;
	}
	ImportToGraphics();
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", material_->diffuse());
	
	btTransform transform = anchor_->GetTransform();

	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->view_matrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->projection_matrix());

	glBindVertexArray(vao_);
	glDrawArrays(GL_POINTS, 0, amount_);
	glBindVertexArray(0);

	emitter_.Emit(particles_, head_ ++);
	if(head_ >= amount_)head_ = 0;
}


