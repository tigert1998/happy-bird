#pragma once

#include "shader.h"
#include "camera.h"
#include "bullet_common.h"

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Cuboid {
private:
	uint32_t vao, vbo, ebo;
	glm::vec3 color_;
	btCollisionShape *shape_;
	Shader *shader_;
	std::vector<uint32_t> indices;
	std::vector<btScalar> vertices;

public:
	Cuboid() = delete;
	Cuboid(btVector3 half_extents, glm::vec3 color = glm::vec3(1, 1, 1));
	~Cuboid();
	btCollisionShape *shape() const;
	void Draw(const Camera *camera, btTransform transform) const;

};