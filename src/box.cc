#include <iostream>
using namespace std;

#include "world.h"
#include "box.h"
#include "vector_utility.h"

// Plain Object
Box::Box(World* world, Shader* shader, const btTransform& transform, glm::vec3 half_extents, Color color):
	DeadObject(world,shader), half_extents_(half_extents) {
	assert(world_);
	is_soft_ = false;
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		0,
		transform,
		new btBoxShape(GLMVec3ToBTVector3(half_extents))
	);
	// create mesh //
	InitMesh();
	ImportToGraphics();
	// patch shader
	if (!shader) {
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
}

void Box::Draw(Camera* camera, const Light* light) {
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	// printf("Box %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	Object::Draw(camera, transform, light);
	btScalar data[16];
	transform.getOpenGLMatrix(data);
}