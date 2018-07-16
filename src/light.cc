#include "light.h"

Light::Light(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	direction_ = direction;
	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
}

void Light::Feed(const std::string& identifier, Shader* shader) const {
	shader->SetUniform<glm::vec3>(identifier + ".direction", direction_);
	shader->SetUniform<glm::vec3>(identifier + ".ambient", ambient_);
	shader->SetUniform<glm::vec3>(identifier + ".diffuse", diffuse_);
	shader->SetUniform<glm::vec3>(identifier + ".specular", specular_);
}
