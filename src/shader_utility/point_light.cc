#include "shader_utility/point_light.h"
#include "math_utility.h"
#include "object.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float intensity, const Attenuation& attenuation):
	position_(position), attenuation_(attenuation), Light(color, intensity) {
}

void PointLight::Attach(Object* target){
	anchor_ = target;
}

LightType PointLight::type() const {
	return LightType::kPoint;
}

glm::vec3 PointLight::position() const {
	if(anchor_)return BTVector3ToGLMVec3(anchor_->GetOrigin());
	return position_;
}

Attenuation PointLight::attenuation() const {
	return attenuation_;
}