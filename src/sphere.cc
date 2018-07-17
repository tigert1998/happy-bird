#include "sphere.h"
#include "world.h"

inline float Sphere::radius(void) const {
	return radius_;
}

Sphere::~Sphere() { }

Sphere::Sphere(World* world, Shader* shader, Material* material, const btTransform& transform, float radius):
	DeadObject(world, shader, material), radius_(radius) {
	assert(world_);
	is_soft_ = false;
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
}

void Sphere::Draw(Camera* camera, const LightCollection* light_collection) {
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	Object::Draw(camera, transform, light_collection);
}
