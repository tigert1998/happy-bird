#include "box.h"
#include "world.h"

// Plain Object
Box::Box(World* world, Shader* shader, float mass, const btTransform& transform, glm::vec3 half_extents, Color color):
	DeadObject(world,shader), half_extents_(half_extents) {
	assert(world_);
	is_soft_ = false;
	color_ = color;
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
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
}

void Box::Draw(Camera* camera, const Lighter* lights) {
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	// printf("Box %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	Object::Draw(camera, transform, lights);
	btScalar data[16];
	transform.getOpenGLMatrix(data);
}