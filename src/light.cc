#include "light.h"

void Light::Feed(const std::string& identifier, Shader *shader) const{
	shader->SetUniform<float>(identifier + ".range", attenuation_.range);
	shader->SetUniform<float>(identifier + ".constant", attenuation_.constant);
	shader->SetUniform<float>(identifier + ".linear", attenuation_.linear);
	shader->SetUniform<float>(identifier + ".quadratic", attenuation_.quadratic);
}

void PointLight::Feed(const std::string& identifier, Shader * shader) const{
	shader->SetUniform<float>(identifier + ".type", kPointLight);
	shader->SetUniform<glm::vec3>(identifier + ".position", position_);
	shader->SetUniform<glm::vec3>(identifier + ".color", color_);
	shader->SetUniform<float>(identifier + ".intensity", intensity_);
}

void ParallelLight::Feed(const std::string& identifier, Shader * shader) const{
	shader->SetUniform<float>(identifier + ".type", kParallelLight);
	shader->SetUniform<glm::vec3>(identifier + ".direction_", direction_);
	shader->SetUniform<glm::vec3>(identifier + ".color", color_);
	shader->SetUniform<float>(identifier + ".intensity", intensity_);
}