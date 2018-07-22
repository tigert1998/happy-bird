#pragma once
#include <vector>

#include "opengl_common.h"
#include "bullet_common.h"
#include "object.h"
#include "camera.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"
#include "controller_utility/keyboard_controller.h"
#include "controller_utility/automation_controller.h"
#include "player_utility/player_collection.h"
#include "stage.h"
#include "temp_collection.h"

#if defined (__WIN32) || defined (__WIN64) || defined (_MSC_VER)
#include "audio.h"
#endif

enum class MoveDirectionType {
	kLeft, kRight, kFront, kBack
};

class World{
	friend Object;
	friend Character;
 protected:
 	// OpenGL util
	GLFWwindow* window_;
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
	Stage stage_;
	PlayerCollection *player_collection_ptr_;
	TempCollection temp_;
 public:
 	// shared
	static bool exit;
	static int height, width;
	static Camera* camera;
	static glm::vec3 global_ambient;
	static LightCollection* light_collection;
	static btVector3 origin;
	static btVector3 forward;
	static btVector3 left;
	static btVector3 up;
	static btScalar character_height; 
	static btScalar bounding_height;
	static btScalar max_speed;

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
	btSoftRigidDynamicsWorld* bt_world(void) const ;
	btBroadphaseInterface* bt_broadphase(void) const;
	btSoftBodyWorldInfo& bt_info();

};