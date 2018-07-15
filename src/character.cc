#include "character.h"
#include "world.h"

// MUST input a ConvexShape pointer
Character::Character(World* world, btCollisionShape* shape): world_(world){
	// Create ghost object
	ghost_object_ = new btPairCachingGhostObject();
	btTransform baseTrans;
	baseTrans.setIdentity();
	ghost_object_->setWorldTransform(baseTrans);
	world->bt_overlapping_paircache_\
		->getOverlappingPairCache()\
		->setInternalGhostPairCallback(\
		new btGhostPairCallback()	);
	ghost_object_->setCollisionShape(shape);
	ghost_object_->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);


	controller_ = new btKinematicCharacterController(
		ghost_object_, 
		dynamic_cast<btConvexShape*>(shape),
		btScalar(0.35)); // step height
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
	controller_->setWalkDirection(dir * step);
}