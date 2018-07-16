#include "world.h"
#include "head.h"
#include "camera.h"

float Head::getRadius(void) const {
	return radius_;
}

Head::~Head() {

}

Head::Head(World* world, Shader* shader,
	const btTransform& transform, float radius, Color color):
	LivingObject(world, shader),
	radius_(radius) {
	assert(world_);
	is_soft_ = false;
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		20,
		transform,
		new btSphereShape(btScalar(radius))
	);
	delegate_ = world_->CreateRigidBody(1, transform, new btSphereShape(btScalar(0.1)));
	delegate_->setCollisionFlags(64); // CF_DISABLE_SPU_COLLISION_PROCESSING = 64 
	// create mesh //
	InitMesh();
	ImportToGraphics();
	// bt_object_->setWorldTransform(transform);
	if (!shader) {
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
	// Bind to new character
	delete character_;
	character_ = new Character(world_, transform, bt_object_->getCollisionShape());
}

void Head::Draw(Camera* camera, const Light* light) {
	btTransform trans = character_->ghost_object_->getWorldTransform();
	delegate_->setWorldTransform(trans);
	Object::Draw(camera, trans, light);
}
