#include "head.h"
#include "world.h"
#include "camera.h"

float Head::radius(void) const {
	return radius_;
}

Head::~Head() { }

Head::Head(World* world, Shader* shader, Material* material,
	const btTransform& transform, float radius):
	LivingObject(world, shader, material),
	radius_(radius) {
	assert(world_);
	is_soft_ = false;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		20,
		transform,
		new btSphereShape(btScalar(radius))
	);
	// create mesh //
	InitMesh();
	ImportToGraphics();
	if (!shader) {
		shader_ = new Shader(common_vert, common_frag);
	}
	// // Bind to new character
	character_ = new CharacterImpl(world_, transform, bt_object_);
	// // Add constraint
	btVector3 pivotInA(0, World::character_height, 0);
	btTransform tmpTrans(btTransform::getIdentity());
	// tmpTrans.setOrigin(btVector3(0, World::character_height, 0));
	btGeneric6DofConstraint* level_constraint = new btGeneric6DofConstraint(*(dynamic_cast<btRigidBody*>(bt_object_)), tmpTrans, true);
	btVector3 translation_lower(1, 0, 1);
	btVector3 translation_upper(-1, World::character_height, -1);
	btVector3 rotation_lower(0, 0, 0);
	btVector3 rotation_upper(0, 0, 0);
	level_constraint->setLinearLowerLimit(translation_lower);
	level_constraint->setLinearUpperLimit(translation_upper);
	level_constraint->setAngularLowerLimit(rotation_lower);
	level_constraint->setAngularUpperLimit(rotation_upper);
	world_->bt_world()->addConstraint((btTypedConstraint*)level_constraint);

}
btVector3 Head::GetOrigin(void){
	return character_->GetDelegate()->getWorldTransform().getOrigin();
}
void Head::Draw(Camera* camera, const LightCollection* light_collection) {
	btCollisionObject* obj = character_->GetDelegate();
	btTransform trans = obj->getWorldTransform();
	Object::Draw(camera, trans, light_collection);
}
