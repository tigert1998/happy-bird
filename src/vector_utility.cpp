#include "vector_utility.h"

btVector3 GLMVec3ToBTVector3(glm::vec3 vector) {
	return btVector3(vector[0], vector[1], vector[2]);
}

glm::vec3 BTVector3ToGLMVec3(btVector3 vector) {
	return glm::vec3(vector[0], vector[1], vector[2]);
}