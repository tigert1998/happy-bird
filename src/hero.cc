#include "hero.h"
#include "world.h"
#include "camera.h"

float Hero::width(void) const {
	return width_;
}
float Hero::height(void) const {
	return height_;
}
float Hero::thickness_(2);

Hero::~Hero() { }

Hero::Hero(World* world, Shader* shader, Material* material,
	const btTransform& transform, float w, float h):
	LivingObject(world, shader, material),
	width_(w), height_(h) {
	assert(world_);
	is_soft_ = false;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		200,
		transform,
		new btBoxShape(btVector3(width_/2, height_/2, thickness_ / 2) )
	);
	// create mesh //
	InitMesh();
	ImportToGraphics();
	if (!shader) {
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
	// // Bind to new character
	character_ = new CharacterImpl(world_, transform, bt_object_);
	// // Add constraint
	btVector3 pivotInA(0, World::character_height, 0);
	btTransform tmpTrans(btTransform::getIdentity());
	// tmpTrans.setOrigin(btVector3(0, World::character_height, 0));
	btGeneric6DofConstraint* level_constraint = new btGeneric6DofConstraint(*(dynamic_cast<btRigidBody*>(bt_object_)), tmpTrans, true);
	btVector3 translation_lower(1, 1, 1);
	btVector3 translation_upper(-1, -1, -1);
	btVector3 rotation_lower(0, 0, 0);
	btVector3 rotation_upper(0, 0, 0);
	level_constraint->setLinearLowerLimit(translation_lower);
	level_constraint->setLinearUpperLimit(translation_upper);
	level_constraint->setAngularLowerLimit(rotation_lower);
	level_constraint->setAngularUpperLimit(rotation_upper);
	world_->bt_world()->addConstraint((btTypedConstraint*)level_constraint);
}
btVector3 Hero::GetOrigin(void){
	return character_->GetDelegate()->getWorldTransform().getOrigin();
}
void Hero::Draw(Camera* camera, const LightCollection* light_collection) {
	btCollisionObject* obj = character_->GetDelegate();
	btTransform trans = obj->getWorldTransform();
	Object::Draw(camera, trans, light_collection);
}
