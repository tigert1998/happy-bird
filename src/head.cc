#include "head.h"
#include "world.h"
#include "camera.h"

Head::Head(World* world, Shader* shader,
	const btTransform& transform, float radius, Color color):
	LivingObject(world, shader),
	radius_(radius){
	assert(world_);
	is_soft_ = false;
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
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
	character_ = new CharacterImpl(world_, transform, bt_object_);
}
btVector3 Head::GetOrigin(void){
	return character_->GetDelegate()->getWorldTransform().getOrigin();
}
void Head::Draw(Camera* camera){
	btCollisionObject* obj = character_->GetDelegate();
	btTransform trans = obj->getWorldTransform();
	// bt_object_->setWorldTransform(trans);
	// printf("Head %f, %f, %f\n", float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	Object::Draw(camera, trans);
}
// Head::Head(World* world, Shader* shader,
// 	const btTransform& transform, float radius, Color color):
// 	LivingObject(world, shader),
// 	radius_(radius){
// 	assert(world_);
// 	is_soft_ = false;
// 	color_ = color;
// 	// initialize physics shape //
// 	bt_object_ = world_->CreateRigidBody(
// 		20,
// 		transform,
// 		new btSphereShape( btScalar(radius) ));
// 	delegate_ = world_->CreateRigidBody(1, transform, new btSphereShape(btScalar(0.1)));
// 	delegate_->setCollisionFlags(64); // CF_DISABLE_SPU_COLLISION_PROCESSING = 64 
// 	// create mesh //
// 	InitMesh();
// 	ImportToGraphics();
// 	// bt_object_->setWorldTransform(transform);
// 	if(!shader){
// 		shader_ = new Shader("shader/common.vert", "shader/common.frag");
// 	}
// 	// Bind to new character
// 	delete character_;
// 	character_ = new Character(world_, transform, bt_object_->getCollisionShape());
// }
// btVector3 Head::GetOrigin(void){
// 	return character_->ghost_object_->getWorldTransform().getOrigin();
// }
// void Head::Draw(Camera* camera){
// 	btTransform trans = character_->ghost_object_->getWorldTransform();
// 	delegate_->setWorldTransform(trans);
// 	// bt_object_->setWorldTransform(trans);
// 	// printf("Head %f, %f, %f\n", float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
// 	Object::Draw(camera, trans);
// }
