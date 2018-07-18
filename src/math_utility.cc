#include "math_utility.h"

btVector3 GLMVec3ToBTVector3(glm::vec3 vector) {
	return btVector3(vector[0], vector[1], vector[2]);
}

glm::vec3 BTVector3ToGLMVec3(const btVector3& vector) {
	return glm::vec3(vector[0], vector[1], vector[2]);
}

btTransform BTTranslate(float x, float y, float z){
	btTransform ret;
	ret.setIdentity();
	ret.setOrigin(btVector3(x,y,z));
	return ret;
}

btTransform BTRotate( const btTransform& base, const btVector3& axis,  float radians){
	btTransform ret = base;
	btMatrix3x3 orn = base.getBasis();
	orn *= btMatrix3x3(btQuaternion(axis, radians));
	ret.setBasis(orn);
	return ret;
}