#include "lighter.h"
void Lighter::AddLight(Light* light){
	lights_.push_back(light);
}
void Lighter::Feed(const std::string& identifier, Shader* shader) const {
	shader->SetUniform<float>(identifier + ".size", lights_.size());
	shader->SetUniform<glm::vec3>(identifier + ".ambient", World::global_ambient);
	for(int i = 0; i < lights_.size(); i++){
		lights_[i]->Feed(identifier + "["+ to_string(i) + "]", shader);
	}
}
