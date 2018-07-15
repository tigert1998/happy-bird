#pragma once
#include "opengl_common.h"
#include "bullet_common.h"
#include "object.h"
#include "camera.h"
#include "shader.h"

#include <functional>
using namespace std::placeholders;

class World{
	friend Object;
 public:
	// OpenGl util
	static int height_, width_;
	GLFWwindow* window_;
	static Camera* camera_;
	// Bullet util
	btDiscreteDynamicsWorld* bt_world_;
	btDefaultCollisionConfiguration* bt_configure_;
	btCollisionDispatcher* bt_dispatcher_;
	btBroadphaseInterface* bt_overlapping_paircache_;
	btSequentialImpulseConstraintSolver* bt_solver_;
	// Object collections
	std::vector<Object*> objects_;

	World(){
		InitPhysics();
		InitGraphics();
		InitScene();
	}
	~World(){
		delete bt_world_;
		delete bt_solver_;
		delete bt_overlapping_paircache_;
		delete bt_dispatcher_;
		delete bt_configure_;
	}
	btRigidBody* createRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape);

	void InitGraphics(void);

	void InitPhysics(void);
	void InitScene(void);
	void Update(void);
	void Run(void);
	static void CursorPosCallback(GLFWwindow *window, double x, double y);

};