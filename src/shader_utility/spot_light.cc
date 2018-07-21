#include "shader_utility/spot_light.h"
#include "object.h"

SpotLight::SpotLight(
	glm::vec3 position, 
	glm::vec3 direction, 
	glm::vec3 color, 
	float intensity, 
	const Attenuation& attenuation, 
	float angle): 
	PointLight(position, color, intensity, attenuation),
	ParallelLight(direction, color, intensity),
	Light(color, intensity),
	angle_(angle) { }

glm::vec3 SpotLight::direction(void) const {
	// override method from parallel light
	if(anchor_){
		return BTVector3ToGLMVec3(anchor_->GetOrigin()) - position_;
	}
	return direction_;
}

void SpotLight::Attach(Object* target){
	anchor_ = target;
}

float SpotLight::angle() const {
	return angle_;
}

LightType SpotLight::type() const {
	return LightType::kSpot;
}