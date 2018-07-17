#include "shader_utility/point_light.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float intensity, const Attenuation& attenuation):
	position_(position), attenuation_(attenuation), Light(color, intensity) {
}

LightType PointLight::type() const {
	return LightType::kPoint;
}

glm::vec3 PointLight::position() const {
	return position_;
}

Attenuation PointLight::attenuation() const {
	return attenuation_;
}