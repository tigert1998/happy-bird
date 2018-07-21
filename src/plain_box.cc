#include "plain_box.h"
#include "world.h"
#include "debug_utility/log.h"

#include <iostream>

// Plain Object
PlainBox::PlainBox(
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
		shader_ = new Shader("shader/cloth.vert", "shader/cloth.frag");
	}
}
void PlainBox::InitMesh(void){
	Object::InitRigidMesh();
}
void PlainBox::ImportToGraphics(void){
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
	// Unbind vao
	glBindVertexArray(0);
}
void PlainBox::Draw(Camera* camera, const LightCollection* light_collection){
	shader_->Use();
	shader_->SetUniform<Material>("uMaterial", *material_);
	shader_->SetUniform<btTransform>("uModelMatrix", GetTransform());
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->view_matrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->projection_matrix());
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}