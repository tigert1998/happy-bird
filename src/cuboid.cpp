#include "cuboid.h"

Cuboid::Cuboid(btVector3 half_extents, glm::vec3 color): color_(color) {	
	using namespace std;

	for (int i = 0; i < 8; i++) {
		vertices.push_back((i & 1) ? half_extents.x() : -half_extents.x());
		vertices.push_back((i & 2) ? half_extents.y() : -half_extents.y());
		vertices.push_back((i & 4) ? half_extents.z() : -half_extents.z());
	}

	indices = {
		0, 3, 1, 0, 3, 2,
		0, 5, 1, 0, 5, 4,
		2, 7, 6, 2, 7, 3,
		4, 7, 5, 4, 7, 6,
		0, 6, 2, 0, 6, 4,
		5, 3, 1, 5, 3, 7
	};

	shape_ = new btBoxShape(half_extents);
	shader_ = new Shader("shader/cuboid.vert", "shader/cuboid.frag");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(btScalar), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(btScalar), (void *) 0);

	glBindVertexArray(0);
}

Cuboid::~Cuboid() {
	delete shape_;
	delete shader_;
}

btCollisionShape *Cuboid::shape() const {
	return shape_;
}

void Cuboid::Draw(const Camera *camera, btTransform transform) const {
	using namespace glm;
	shader_->Use();
	shader_->SetUniform<vec3>("uColor", color_);
	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<mat4>("uViewMatrix", camera->GetViewMatrix());
	shader_->SetUniform<mat4>("uProjectionMatrix", camera->GetProjectionMatrix());

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

