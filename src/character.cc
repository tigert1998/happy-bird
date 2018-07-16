#include <iostream>

#include <iostream>

#include "character.h"
#include "world.h"

// MUST input a ConvexShape pointer
Character::Character(World* world, const btTransform& startTransform, btCollisionShape* shape): world_(world){
	// Create ghost object
	ghost_object_ = new btPairCachingGhostObject();
	ghost_object_->setWorldTransform(startTransform);
	world->bt_overlapping_paircache_\
		->getOverlappingPairCache()\
		->setInternalGhostPairCallback(\
		new btGhostPairCallback()	);
	ghost_object_->setCollisionShape(shape);
	ghost_object_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	// ghost_object_->setActivationState(DISABLE_DEACTIVATION);

	controller_ = new btKinematicCharacterController(
		ghost_object_, 
		dynamic_cast<btConvexShape*>(shape),
		btScalar(0.35)); // step height
	controller_->setWalkDirection(btVector3(0,0,0));
	controller_->setUp(btVector3(0,1,0));
	world_->bt_world_->addCollisionObject(
		ghost_object_,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	world_->bt_world_->addAction(controller_);

}
Character::~Character(){
	delete controller_;
	delete ghost_object_;
}
void Character::ResetMove(void){
	controller_->setWalkDirection(btVector3(0,0,0));
}
void Character::Move(bool forward, float step){
	// std::cout << "Move" << std::endl;
	btTransform trans = ghost_object_->getWorldTransform();
	btVector3 forwardDir = trans.getBasis()[1];
	step = 1;
	controller_->setWalkDirection(forwardDir * step * (forward?1:-1));
}
void Character::Rotate(bool left, float step){
	// std::cout << "Rotate" << std::endl;
	btMatrix3x3 orn = ghost_object_->getWorldTransform().getBasis();
	// std::cout << "Basis: " << orn[0][0] << ", " << orn[0][1] << ", " << orn[0][2] << std::endl 
	// 	<< "       " << orn[1][0] << ", " << orn[1][1] << ", " << orn[1][2] << std::endl
	// 	<< "       " << orn[2][0] << ", " << orn[2][1] << ", " << orn[2][2] << std::endl;
	orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),0.01 * (left?-1:1) ));
	step = 1;
	ghost_object_->getWorldTransform().setBasis(orn);
}
void Character::Jump(float step){
	// btTransform trans = ghost_object_->getWorldTransform();
	// btVector3 upDir = trans.getBasis()[1];
	controller_->jump();
}
