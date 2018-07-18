#pragma once

#include "shader_utility/material.h"

class PureColorMaterial: public Material {
private:
	glm::vec3 diffuse_, specular_;

public:
	PureColorMaterial() = delete;
	PureColorMaterial(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	glm::vec3 diffuse() const;
	glm::vec3 specular() const;
	MaterialType type() const;
};