#include <iostream>

#include <iostream>

#include "character.h"
#include "world.h"

// MUST input a ConvexShape pointer
Character::Character(World* world, btCollisionShape* shape): world_(world){
	// Create ghost object
	std::cout << "Create btPairCachingGhostObject" << std::endl;
	ghost_object_ = new btPairCachingGhostObject();
	btTransform baseTrans;
	baseTrans.setIdentity();
	std::cout << "Settint Ghost" << std::endl;
	ghost_object_->setWorldTransform(baseTrans);
	world->bt_overlapping_paircache_\
		->getOverlappingPairCache()\
		->setInternalGhostPairCallback(\
		new btGhostPairCallback()	);
	ghost_object_->setCollisionShape(shape);
	ghost_object_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	ghost_object_->setActivationState(DISABLE_DEACTIVATION);

	std::cout << "Create Controller" << std::endl;
	controller_ = new btKinematicCharacterController(
		ghost_object_, 
		dynamic_cast<btConvexShape*>(shape),
		btScalar(0.35)); // step height
	std::cout << "Add to world" << std::endl;
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
void Character::Move(const btVector3& dir, float step){
	std::cout << "Moving towards: " << dir[0] << ", " << dir[1] << ", " <<dir[2] << " with time " << step << std::endl;
	controller_->setWalkDirection(dir * step);
}
void Character::Jump(float step){
	std::cout << "Jumping" << std::endl;
	controller_->jump();
}