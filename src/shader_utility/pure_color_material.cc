#include "shader_utility/pure_color_material.h"

PureColorMaterial::PureColorMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess): 
	diffuse_(diffuse), specular_(specular), Material(shininess) { }
	
glm::vec3 PureColorMaterial::diffuse() const {
	return diffuse_;
}

glm::vec3 PureColorMaterial::specular() const {
	return specular_;
}

MaterialType PureColorMaterial::type() const {
	return MaterialType::kPureColor;
}