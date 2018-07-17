#pragma once

#include <glm/glm.hpp>

class Material {
private:
	glm::vec3 diffuse_, specular_;
	float shininess_;

public:
	Material() = delete;
	Material(glm::vec3 diffuse, glm::vec3 specular, float shininess);
	glm::vec3 diffuse() const;
	glm::vec3 specular() const;
	float shininess() const;
};