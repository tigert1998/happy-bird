#include "shader_utility/light.h"

Light::Light(glm::vec3 color, float intensity) {
	color_ = color;
	intensity_ = intensity;
}

glm::vec3 Light::color() const {
	return color_;
}

float Light::intensity() const {
	return intensity_;
}
