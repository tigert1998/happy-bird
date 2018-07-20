#include "box.h"
#include "world.h"

#include <iostream>

// Plain Object
Box::Box(
	World* world,
	Shader* shader,
	Material* material,
	float mass, 
	const btTransform& transform, 
	glm::vec3 half_extents
): DeadObject(world, shader, material), half_extents_(half_extents) {
	assert(world_);
	is_soft_ = false;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		mass,
		transform,
		new btBoxShape(GLMVec3ToBTVector3(half_extents))
	);
	// create mesh //
	InitMesh();
	ImportToGraphics();
	// patch shader
	if (!shader) {
		shader_ = new Shader(common_vert, common_frag);
	}
}

void Box::Draw(Camera* camera, const LightCollection* light_collection) {
	std::cout << "[Box::Draw(Camera*, const LightCollection*)]" << std::endl;
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	Object::Draw(camera, transform, light_collection);
}