#include "cuboid.h"
#include "sphere.h"

#include <stdio.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "bullet_common.h"

using namespace std;
using namespace glm;

int width = 800, height = 600;
Camera *camera;

void CursorPosCallback(GLFWwindow *window, double x, double y) {
	static double mouse_x, mouse_y;
	static bool entered = false;
	double new_x = x / width - 0.5;
	double new_y = y / height - 0.5;
	if (entered) {
		camera->Rotate(mouse_x - new_x, mouse_y - new_y);
	}
	mouse_x = new_x;
	mouse_y = new_y;
	entered = true;
}

int main(int argc, char** argv)
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(width, height, "", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	camera = new Camera(glm::vec3(5, 51, 5), (double) width / (double) height);

	int i;
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamic_world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamic_world->setGravity(btVector3(0, -10, 0));

	btAlignedObjectArray<btCollisionShape*> collision_shapes;

	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	auto ground = Cuboid(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	auto sphere = Sphere(1, glm::vec3(1, 0, 0));

	{
		collision_shapes.push_back(ground.shape());

		btTransform ground_transform;
		ground_transform.setIdentity();
		ground_transform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		bool isDynamic = (mass != 0.f);

		btVector3 local_inertia(0, 0, 0);
		if (isDynamic)
			ground.shape()->calculateLocalInertia(mass, local_inertia);

		btDefaultMotionState* motion_state = new btDefaultMotionState(ground_transform);
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, ground.shape(), local_inertia);
		btRigidBody* body = new btRigidBody(rb_info);

		dynamic_world->addRigidBody(body);
	}

	{

		collision_shapes.push_back(sphere.shape());

		btTransform start_transform;
		start_transform.setIdentity();
		start_transform.setOrigin(btVector3(2, 10, 0));

		btScalar mass(1.f);

		bool isDynamic = (mass != 0.f);

		btVector3 local_inertia(0, 0, 0);
		if (isDynamic)
			sphere.shape()->calculateLocalInertia(mass, local_inertia);

		btDefaultMotionState* motion_state = new btDefaultMotionState(start_transform);
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, sphere.shape(), local_inertia);
		btRigidBody* body = new btRigidBody(rb_info);

		dynamic_world->addRigidBody(body);
	}

	float last_time = 0, current_time = 0;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		current_time = glfwGetTime();
		float delta_time = current_time - last_time;
		last_time = current_time;

		dynamic_world->stepSimulation(delta_time);
		btTransform transform;

		btRigidBody::upcast(dynamic_world->getCollisionObjectArray()[0])->getMotionState()->getWorldTransform(transform);
		ground.Draw(camera, transform);

		btRigidBody::upcast(dynamic_world->getCollisionObjectArray()[1])->getMotionState()->getWorldTransform(transform);
		sphere.Draw(camera, transform);

		printf("sphere %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
