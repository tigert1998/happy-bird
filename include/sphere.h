#pragma once

#include "shader.h"
#include "camera.h"
#include "collision_shape_to_triangle_mesh.h"
#include "bullet_common.h"

#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Sphere {
private:
	uint32_t vao, vbo, ebo;
	double radius_;
	btSphereShape *shape_;
	std::vector<float> vertices_;
	std::vector<float> normals_;
	std::vector<uint32_t> indices_;
	Shader *shader_;
	glm::vec3 color_;

public:
	Sphere() = delete;
	Sphere(double radius, glm::vec3 color = glm::vec3(1, 1, 1));
	~Sphere();

	btSphereShape *shape() const;

	void Draw(const Camera *camera, btTransform transform) const;
};