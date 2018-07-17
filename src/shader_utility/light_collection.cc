#include "shader_utility/light_collection.h"

LightCollection::LightCollection(glm::vec3 ambient): ambient_(ambient) { }

void LightCollection::PushBack(const Light* light_ptr) {
	light_ptrs_.push_back(light_ptr);
}

const Light* LightCollection::operator[](int i) const {
	return light_ptrs_[i];
}

int LightCollection::Size() const {
	return light_ptrs_.size();
}

glm::vec3 LightCollection::ambient() const {
	return ambient_;
}