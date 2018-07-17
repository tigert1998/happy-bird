#include "shader_utility/material.h"

Material::Material(glm::vec3 diffuse, glm::vec3 specular, float shininess) {
	diffuse_ = diffuse;
	specular_ = specular;
	shininess_ = shininess;
}

glm::vec3 Material::diffuse() const {
	return diffuse_;
}
	
glm::vec3 Material::specular() const {
	return specular_;
}
	
float Material::shininess() const {
	return shininess_;
}
