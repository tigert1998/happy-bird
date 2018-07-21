#include <iostream>
using std::cout;
using std::endl;

#include "particle.h"
#include "random.h"
#include "math_utility.h"
#include "opengl_common.h"
#include "shader_utility/pure_color_material.h"

ParticleConfig::ParticleConfig(glm::vec3 v, Color color, int intFlag): 
		major_velocity(v),
		major_color(color){
	ParticleFlag flags = static_cast<ParticleFlag>(intFlag) ;
	switch(flags & 0x03){
		case (kGravityParticle) : // Gravity
			acceleration = glm::vec3(0, -0.0001, 0);
			break;
		case (kFlameParticle) : // flame
			acceleration = glm::vec3(0, 0.0001, 0);
			break;
		default :
			acceleration = glm::vec3(0, 0, 0);
	}
	switch(flags & 0x0c){
		case (kLargeParticle):
			major_radius = 12;
			variance_radius = 6;
			break;
		case (kMediumParticle):
			major_radius = 5;
			variance_radius = 3;
			break;
		default:
			major_radius = 3.5;
			variance_radius = 1;
	}
	switch(flags & 0x10){
		case(kAmbientParticle):
			variance_velocity = std::max(std::max(fabs(major_velocity[0]),fabs(major_velocity[1])),fabs(major_velocity[2]));
			break;
		default:
			variance_velocity = 0;
	}
	switch(flags & 0x60){
		case(kGradualColorParticle):
			gradual = true;
			delta_color = glm::vec3(-major_color[0]/3, -major_color[1]/3, -major_color[2]/3);
			break;
		case(kRandomColorParticle):
			gradual = false;
			delta_color = glm::vec3(-major_color[0], -major_color[1], -major_color[2]);
			break;
		default:
			gradual = true;
			delta_color = glm::vec3(0, 0, 0);
	}
	if(flags & kJitterParticle){
		jitter = true;
	}
	else{
		jitter = false;
	}
	if(flags & kMockFlame){
		mock_flame = true;
	}
	else{
		mock_flame = false;
	}
}

// ParticleEmitter::ParticleEmitter():
// 	major_velocity(0,0,0.02),
// 	acceleration_(0,0,0),
// 	variance_velocity(0.02),
// 	major_radius_(5),
// 	variance_radius_(3),
// 	major_color(color::White()),
// 	delta_color(-color::White()),
// 	gradual(false),
// 	interval_(0.07){
// 	timer_ = Timer::New();
// }
ParticleEmitter::ParticleEmitter(ParticleConfig config):config_(config){ }
void ParticleEmitter::Emit(std::vector<ParticleInfo>::iterator curslot, glm::vec3 p){
	if(config_.mock_flame){
		glm::vec3 offset(Random::QueryFloatRandom(0,1) * Random::QueryFloatRandom(0,1) * 7, 0, Random::QueryFloatRandom(0,1) * Random::QueryFloatRandom(0,1) * 7 );
		float seed = sqrt(offset[0] / 7 + offset[1] / 7);
		if((int)(seed * 7) % 2 == 0)offset[0] *= -1;
		if((int)(seed * 13) % 2 == 1)offset[2] *= -1;
		glm::vec3 position = p + offset; // near center
		glm::vec3 v = config_.major_velocity * (1 - seed);
		Color color = config_.major_color * ((2 - seed) * (2 - seed) / 2.0f);
		float radius = config_.major_radius * (( 2 - seed)* (2 - seed));
		*curslot = ParticleInfo(
			position,
			v,
			config_.acceleration,
			radius,
			color,
			true
		);
		return ;
	}
	// system("pause"); 
	glm::vec3 v = config_.major_velocity;
	float min_component = 0.5 + std::min(std::min(fabs(v[0]),fabs(v[1])),fabs(v[2])); // min has largest var, others small var
	float factor = Random::QueryFloatRandom(-config_.variance_velocity, config_.variance_velocity);
	v[0] += factor * min_component / (v[0]+0.5);
	factor = Random::QueryFloatRandom(-config_.variance_velocity, config_.variance_velocity);
	v[1] += factor * min_component / (v[1]+0.5);
	factor = Random::QueryFloatRandom(-config_.variance_velocity, config_.variance_velocity);
	v[2] += factor * min_component / (v[2]+0.5);
	Color color = config_.major_color;
	factor = Random::QueryFloatRandom(0, 1);
	color[0] += config_.delta_color[0] * factor;
	if(!config_.gradual)factor = Random::QueryFloatRandom(0, 1);
	color[1] += config_.delta_color[1] * factor;
	if(!config_.gradual)factor = Random::QueryFloatRandom(0, 1);
	color[2] += config_.delta_color[2] * factor;
	if(config_.jitter){
		p += glm::vec3(Random::QueryFloatRandom(-3,3), 0, Random::QueryFloatRandom(-3,3));
	}
	*curslot = ParticleInfo(
		p,
		v,
		config_.acceleration,
		config_.major_radius + Random::QueryFloatRandom(0, config_.variance_radius),
		color
	);
}

Particle::Particle(
		World* world,
		Shader* shader,
		Material* material,
		const btVector3& position,
		glm::vec3 velocity,
		int flags,
		int amount,
		float interval,
		float duration
):
		Object(world, shader, material, 7, false),
		position_(position),
		amount_(amount), 
		particles_(amount), 
		interval_(interval),
		duration_(duration),
		emitter_(ParticleConfig(velocity, dynamic_cast<PureColorMaterial*>(material)->diffuse(), flags)){
	assert(world_);
	LOG();
	interval_timer_ = Timer::New();
	duration_timer_ = Timer::New();
	data_.resize(amount_ * 7);
	if(!shader_)shader_ = new Shader("shader/particle.vert", "shader/particle.frag");
	InitMesh();

}
btVector3 Particle::GetOrigin(void){
	if(!(anchor_.lock())){
		return position_;
	}
	return Object::GetOrigin();
}
void Particle::InitMesh(void){
	btVector3 p = GetOrigin();
	for(auto slot = particles_.begin(); slot != particles_.end(); slot ++){
		emitter_.Emit(slot,BTVector3ToGLMVec3(p));
	}
	slot_ = particles_.begin();
}
void Particle::ImportToGraphics(){
	glBindVertexArray(vao_); 
	// No bind ebo
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data_.size(), data_.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
void Particle::Draw(Camera* camera, const LightCollection* lights) {
	static Timer::TimingId timerId = Timer::New();
	float delta_time = Timer::QueryAndPin(timerId) * 2000;
	// static float last_time = glfwGetTime(), current_time;
	// current_time = glfwGetTime();
	// float delta_time = (current_time - last_time) * 500;
	// cout << amount_ <<  " -> Delta: " << delta_time << endl; 
	// last_time = current_time;
	for(int i = 0; i < amount_; i++){
		particles_[i].Update(delta_time);
		data_[7*i + 0] = particles_[i].position[0];
		data_[7*i + 1] = particles_[i].position[1];
		data_[7*i + 2] = particles_[i].position[2];
		data_[7*i + 3] = particles_[i].radius;
		data_[7*i + 4] = particles_[i].color[0];
		data_[7*i + 5] = particles_[i].color[1];
		data_[7*i + 6] = particles_[i].color[2];
	}
	ImportToGraphics();
	shader_->Use();
	// shader_->SetUniform<glm::vec3>("uColor", dynamic_cast<PureColorMaterial*>(material_)->diffuse());
	
	btTransform transform;
	transform.setIdentity();

	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->view_matrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->projection_matrix());

	glBindVertexArray(vao_);
	glDrawArrays(GL_POINTS, 0, amount_);
	glBindVertexArray(0);

	if(Timer::Query(interval_timer_) >= interval_ && ( duration_ <= 0 || Timer::Query(duration_timer_) < duration_)){
		Timer::Pin(interval_timer_);
		btVector3 p = GetOrigin();
		emitter_.Emit(slot_, BTVector3ToGLMVec3(p));		
		slot_ ++;
		if(slot_ >= particles_.end())slot_ = particles_.begin();
	}
}


