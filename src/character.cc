#include <iostream>
using std::cout;
using std::endl;

#include "character.h"
#include "world.h"

// MUST input a ConvexShape pointer
CharacterBullet::CharacterBullet(World* world, const btTransform& startTransform, btCollisionObject* obj){
	world_ = world;
	btCollisionShape* shape = obj->getCollisionShape();
	// Create ghost object
	ghost_object_ = new btPairCachingGhostObject();
	ghost_object_->setWorldTransform(startTransform);
	world->bt_broadphase()\
		->getOverlappingPairCache()\
		->setInternalGhostPairCallback(\
		new btGhostPairCallback()	);
	ghost_object_->setCollisionShape(shape);
	ghost_object_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	controller_ = new btKinematicCharacterController(
		ghost_object_, 
		dynamic_cast<btConvexShape*>(shape),
		btScalar(0.35)); // step height
	controller_->setWalkDirection(btVector3(0,0,0));
	controller_->setUp(btVector3(0,1,0));
	world_->bt_world()->addCollisionObject(
		ghost_object_,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	world_->bt_world()->addAction(controller_);

	btRigidBody* body = world_->CreateRigidBody(1, startTransform, new btSphereShape(btScalar(0.1)));
	body->setCollisionFlags(64); // CF_DISABLE_SPU_COLLISION_PROCESSING = 64 
	delegate_ = static_cast<btCollisionObject*>(body);
}
CharacterBullet::~CharacterBullet(){
	delete controller_;
	delete ghost_object_;
}
btCollisionObject* CharacterBullet::GetDelegate(void){
	UpdateDelegate();
	return delegate_;
}
void CharacterBullet::UpdateDelegate(void){
	btTransform trans = ghost_object_->getWorldTransform();
	delegate_->setWorldTransform(trans);
}
void CharacterBullet::ResetMove(void){
	controller_->setWalkDirection(btVector3(0,0,0));
	UpdateDelegate();
}
void CharacterBullet::Move(bool forward, float step){
	// std::cout << "Move" << std::endl;
	btTransform trans = ghost_object_->getWorldTransform();
	btVector3 forwardDir = trans.getBasis()[1];
	step = 1;
	controller_->setWalkDirection(forwardDir * step * (forward?1:-1));
	UpdateDelegate();
}
void CharacterBullet::Rotate(bool left, float step){
	// std::cout << "Rotate" << std::endl;
	btMatrix3x3 orn = ghost_object_->getWorldTransform().getBasis();
	orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),0.01 * (left?-1:1) ));
	step = 1;
	ghost_object_->getWorldTransform().setBasis(orn);
	UpdateDelegate();
}
void CharacterBullet::Jump(float step){
	controller_->jump();
	UpdateDelegate();
}

CharacterImpl::CharacterImpl(World* world, const btTransform& transform, btCollisionObject* obj){
	world_ = world;
	object_ = obj;
}
CharacterImpl::~CharacterImpl(){ }
btCollisionObject* CharacterImpl::GetDelegate(void){
	return object_;
}
void CharacterImpl::Move(bool forward, float step){
	btTransform trans = object_->getWorldTransform();
	btVector3 forwardDir = trans.getBasis()[2].normalize();
	cout << "Forward dir: " << forwardDir[0] << ", " << forwardDir[1] << ", " << forwardDir[2] << endl;
	step = 10;
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	body->setLinearVelocity(body->getLinearVelocity() + forwardDir * step * (forward?1:-1));
}
void CharacterImpl::Rotate(bool left, float step){
	// btMatrix3x3 orn = object_->getWorldTransform().getBasis();
	// orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),0.01 * (left?-1:1) ));
	// step = 10;
	// object_->getWorldTransform().setBasis(orn);
	btTransform trans = object_->getWorldTransform();
	btVector3 leftDir = trans.getBasis()[0].normalize();
	cout << "Left dir: " << leftDir[0] << ", " << leftDir[1] << ", " << leftDir[2] << endl;
	step = 10;
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	body->setLinearVelocity(body->getLinearVelocity() + leftDir * step * (left?1:-1));
}
void CharacterImpl::Jump(float step){
	step = 10;
	btRigidBody* body = dynamic_cast<btRigidBody*>(object_);
	body->setLinearVelocity(body->getLinearVelocity() + btVector3(0,1,0) * step);
}
void CharacterImpl::ResetMove(void){
	dynamic_cast<btRigidBody*>(object_)->setLinearVelocity(btVector3(0,0,0));
	return ;
}