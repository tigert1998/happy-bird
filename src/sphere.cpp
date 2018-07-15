#include "sphere.h"

Sphere::Sphere(double radius, glm::vec3 color): radius_(radius), color_(color) {
	static btAlignedObjectArray<btVector3> vertex_positions;
	static btAlignedObjectArray<btVector3> vertex_normals;
	static btAlignedObjectArray<int> indices_out;

	shape_ = new btSphereShape(btScalar(radius));
	btTransform transform;
	transform.setIdentity();
	CollisionShapeToTriangleMesh(shape_, transform, vertex_positions, vertex_normals, indices_out);
	vertices_.resize(vertex_positions.size() * 3);
	normals_.resize(vertex_normals.size() * 3);
	for (int i = 0; i < vertex_positions.size(); i++) {
		vertices_[i * 3 + 0] = vertex_positions[i].x();
		vertices_[i * 3 + 1] = vertex_positions[i].y();
		vertices_[i * 3 + 2] = vertex_positions[i].z();
		normals_[i * 3 + 0] = vertex_normals[i].x();
		normals_[i * 3 + 1] = vertex_normals[i].y();
		normals_[i * 3 + 2] = vertex_normals[i].z();
	}
	indices_.resize(indices_out.size());
	for (int i = 0; i < indices_.size(); i++) indices_[i] = indices_out[i];	

	shader_ = new Shader("shader/sphere.vert", "shader/sphere.frag");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(btScalar), vertices_.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), indices_.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(btScalar), (void *) 0);

	glBindVertexArray(0);
}

Sphere::~Sphere() {
	delete shape_;
	delete shader_;
}

btSphereShape *Sphere::shape() const {
	return shape_;
}

void Sphere::Draw(const Camera *camera, btTransform transform) const {
	using namespace glm;
	shader_->Use();
	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<vec3>("uColor", color_);
	shader_->SetUniform<mat4>("uViewMatrix", camera->GetViewMatrix());
	shader_->SetUniform<mat4>("uProjectionMatrix", camera->GetProjectionMatrix());

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}