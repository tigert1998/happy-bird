#include "world.h"
#include "head.h"
#include "camera.h"

Head::Head(World* world, Shader* shader,
	const btTransform& transform, float radius, Color color):
	LivingObject(world, shader),
	radius_(radius){
	assert(world_);
	is_soft_ = false;
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->createRigidBody(
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
	// Bind to new character
	delete character_;
	character_ = new Character(world_, transform, bt_object_->getCollisionShape());
}
void Head::Draw(Camera* camera){
	btTransform trans = character_->ghost_object_->getWorldTransform();
	Object::Draw(camera, trans);
	auto origin = BTVector3ToGLMVec3(trans.getOrigin());
	printf("head origin = %lf, %lf, %lf\n", origin[0], origin[1], origin[2]);
	printf("camera front = %lf, %lf, %lf\n", camera->front()[0], camera->front()[1], camera->front()[2]);
	printf("camera center = %lf, %lf, %lf\n", camera->center()[0], camera->center()[1], camera->center()[2]);
	printf("camera position = %lf, %lf, %lf\n", camera->position()[0], camera->position()[1], camera->position()[2]);

}