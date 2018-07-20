#include "sphere.h"
#include "world.h"
#include "debug_utility/log.h"

inline float Sphere::radius(void) const {
	return radius_;
}

Sphere::~Sphere() { }

Sphere::Sphere(World* world, Shader* shader, Material* material, const btTransform& transform, float radius):
	Object(world, shader, material, 6, false), radius_(radius) {
	assert(world_);
	std::cout << "InitSphere" << std::endl;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		20,
		transform,
		new btSphereShape( btScalar(radius) ));
	// create mesh //
	Object::InitRigidMesh();
	ImportToGraphics();
	if(!shader){
		shader_ = new Shader(common_vert, common_frag);
	}
}
void Sphere::ImportToGraphics(void){
	glBindVertexArray(vao_);
	// Bind indice
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), indices_.data(), GL_STATIC_DRAW);
	// Bind data
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, data_.size() * sizeof(float), data_.data(), GL_STATIC_DRAW);	
	// Basic Attrib: vertex coords
	glVertexAttribPointer(0, 3, GL_FLOAT, false, stride_ * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// Normal attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, false, stride_ * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind vao
	glBindVertexArray(0);
}