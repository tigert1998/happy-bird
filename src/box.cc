#include "box.h"
#include "world.h"

#include <iostream>

// Plain Object
Box::Box(
		World* world,
		Shader* shader,
		Material* material,
		const btTransform& transform, 
		glm::vec3 half_extents,
		float mass 
	):
		Object(world, shader, material, 6, false), 
		half_extents_(half_extents),
		mass_(mass) {
	assert(world_);
	std::cout << "InitBox" << std::endl;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		mass,
		transform,
		new btBoxShape(GLMVec3ToBTVector3(half_extents))
	);
	// create mesh //
	InitMesh(); // use default stride = 6
	ImportToGraphics();
	// patch shader
	if (!shader) {
		shader_ = new Shader(common_vert, common_frag);
	}
}
void Box::InitMesh(void){
	Object::InitRigidMesh();
}
void Box::ImportToGraphics(void){
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