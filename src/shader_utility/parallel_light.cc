#include "shader_utility/parallel_light.h"

ParallelLight::ParallelLight(glm::vec3 direction, glm::vec3 color, float intensity):
	direction_(direction), Light(color, intensity) {
}

LightType ParallelLight::type() const {
	return LightType::kParallel;
}

glm::vec3 ParallelLight::direction() const {
	return direction_;
}