#include "shader_utility/spot_light.h"

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

float SpotLight::angle() const {
	return angle_;
}

LightType SpotLight::type() const {
	return LightType::kSpot;
}