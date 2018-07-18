#include "shader_utility/material.h"

Material::Material(float shininess) {
	shininess_ = shininess;
}

float Material::shininess() const {
	return shininess_;
}
