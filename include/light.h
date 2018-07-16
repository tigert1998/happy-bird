#pragma once

#include "shader.h"

#include <string>

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 direction_, ambient_, diffuse_, specular_;

public:
	Light() = delete;
	Light(
		glm::vec3 direction,
		glm::vec3 ambient = glm::vec3(0.2, 0.2, 0.2), 
		glm::vec3 diffuse = glm::vec3(0.5, 0.5, 0.5), 
		glm::vec3 specular = glm::vec3(1, 1, 1)
	);
	void Feed(const std::string& identifier, Shader *shader) const;
};