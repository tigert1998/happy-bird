#include "head.h"
#include "world.h"
#include "camera.h"

float Head::radius(void) const {
	return radius_;
}

Head::~Head() { }

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
	// create mesh //
	InitMesh();
	ImportToGraphics();
	// bt_object_->setWorldTransform(transform);
	if (!shader) {
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
	// Bind to new character
	character_ = new CharacterImpl(world_, transform, bt_object_);
	// Add constraint
	btVector3 pivotInA(0, World::character_height, 0);
	btTransform tmpTrans(btTransform::getIdentity());
	// tmpTrans.setOrigin(pivotInA);
		// whether ref to frameInB
	btGeneric6DofConstraint* level_constraint = new btGeneric6DofConstraint(*(dynamic_cast<btRigidBody*>(bt_object_)), tmpTrans, true);
	btVector3 translation_lower(1, 0, 1);
	btVector3 translation_upper(-1, World::character_height / 2, -1);
	btVector3 rotation_lower(0, 0, 0);
	btVector3 rotation_upper(0, 0, 0);
	level_constraint->setLinearLowerLimit(translation_lower);
	level_constraint->setLinearUpperLimit(translation_upper);
	level_constraint->setAngularLowerLimit(rotation_lower);
	level_constraint->setAngularUpperLimit(rotation_upper);
	// btTypedConstraint* level_constraint = new btPoint2PointConstraint(*body, pivotInA);
	world_->bt_world()->addConstraint((btTypedConstraint*)level_constraint);

}
btVector3 Head::GetOrigin(void){
	return character_->GetDelegate()->getWorldTransform().getOrigin();
}
void Head::Draw(Camera* camera, const Lighter* lights) {
	btCollisionObject* obj = character_->GetDelegate();
	btTransform trans = obj->getWorldTransform();
	Object::Draw(camera, trans, lights);
}
