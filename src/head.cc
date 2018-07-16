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
}
btVector3 Head::GetOrigin(void){
	return character_->GetDelegate()->getWorldTransform().getOrigin();
}
void Head::Draw(Camera* camera, const Light* light) {
	btCollisionObject* obj = character_->GetDelegate();
	btTransform trans = obj->getWorldTransform();
	Object::Draw(camera, trans, light);
}
