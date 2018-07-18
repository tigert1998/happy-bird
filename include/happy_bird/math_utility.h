#pragma once

#include <glm/glm.hpp>
#include "bullet_common.h"

btVector3 GLMVec3ToBTVector3(glm::vec3 vector);

glm::vec3 BTVector3ToGLMVec3(const btVector3& vector);

btTransform BTTranslate(float x, float y, float z);

btTransform BTRotate( const btTransform& transform, const btVector3& axis,  float radians);
