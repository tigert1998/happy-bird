#include "sphere.h"
#include "world.h"

inline float Sphere::getRadius(void) {
	return radius_;
	}

Sphere::~Sphere() {

}

Sphere::Sphere(World* world, Shader* shader, const btTransform& transform, float radius, Color color):
	DeadObject(world,shader), radius_(radius) {
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
}

void Sphere::Draw(Camera* camera, const Light* light) {
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	// printf("Sphere %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	Object::Draw(camera, transform, light);
}
