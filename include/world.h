#pragma once
#include <vector>

#include "opengl_common.h"
#include "bullet_common.h"
#include "object.h"
#include "character.h"
#include "camera.h"
#include "shader.h"

class World{
	enum {
		kLeft = 0,
		kRight = 1,
		kFront = 2,
		kBack = 3
	} MoveDirectionType;
	friend Object;
	friend Character;
 public:
	// OpenGl util
	static int height_, width_;
	GLFWwindow* window_;
	static Camera* camera_;
	// Bullet util
	btSoftRigidDynamicsWorld* bt_world_;
	btSoftBodyRigidBodyCollisionConfiguration* bt_configure_;
	// btDefaultCollisionConfiguration* bt_configure_;
	btCollisionDispatcher* bt_dispatcher_;
	btBroadphaseInterface* bt_overlapping_paircache_;
	btSequentialImpulseConstraintSolver* bt_solver_;
	btSoftBodySolver* bt_soft_solver_;
	btSoftBodyWorldInfo bt_soft_info_;
	// Object collections
	std::vector<Object*> objects_;
	// controller
	Character* character_;

	World();
	~World();
	btRigidBody* CreateRigidBody(btScalar mass, const btTransform&, btCollisionShape* shape);

	void InitGraphics(void);

	void InitPhysics(void);
	void InitScene(void);
	void Update(void);
	void Run(void);
	static void CursorPosCallback(GLFWwindow *window, double x, double y);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void ProcessInput(void);
};