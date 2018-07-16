#pragma once

#include "bullet_common.h"

#include <glm/glm.hpp>

btVector3 GLMVec3ToBTVector3(glm::vec3 vector);

glm::vec3 BTVector3ToGLMVec3(btVector3 vector);
