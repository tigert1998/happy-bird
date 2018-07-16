#include "world.h"
#include "head.h"
#include "camera.h"

Head::Head(World* world, Shader* shader,
	const btTransform& transform, float radius, Color color):
	LivingObject(world, shader),
	radius_(radius){
	assert(world_);
	is_soft_ = false;
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->createRigidBody(
		20,
		transform,
		new btSphereShape( btScalar(radius) ));
	// create mesh //
	InitMesh();
	ImportToGraphics();
	// bt_object_->setWorldTransform(transform);
	if(!shader){
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
	// Bind to new character
	delete character_;
	character_ = new Character(world_, transform, bt_object_->getCollisionShape());
}
void Head::Draw(Camera* camera){
	// btTransform transform;
	// btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	// printf("Head %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	// Object::Draw(camera);
	// return ;
	btTransform trans = character_->ghost_object_->getWorldTransform();
	// printf("Head %f, %f, %f\n", float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	Object::Draw(camera, trans);
}