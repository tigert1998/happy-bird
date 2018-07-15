#include "world.h"

int World::height_ = 600;
int World::width_ = 800;
Camera* World::camera_ = new Camera(glm::vec3(25, 51, 25), (double) World::width_ / (double) World::height_);

btRigidBody* World::createRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	bt_world_->addRigidBody(body);

	return body;
}

void World::InitGraphics(void){
	glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window_ = glfwCreateWindow(width_, height_, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window_);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	// camera_ = new Camera(glm::vec3(5, 51, 5), (double) width_ / (double) height_);
	// glfwSetCursorPosCallback(window_, std::bind(World::CursorPosCallback, _1,_2,_3,camera_,width_,height_) );
	glfwSetCursorPosCallback(window_, World::CursorPosCallback );
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void World::InitPhysics(void){
	bt_configure_ = new btDefaultCollisionConfiguration();
	bt_dispatcher_ = new btCollisionDispatcher(bt_configure_);
	bt_overlapping_paircache_ = new btDbvtBroadphase();
	bt_solver_ = new btSequentialImpulseConstraintSolver;
	bt_world_ = new btDiscreteDynamicsWorld(bt_dispatcher_,
		bt_overlapping_paircache_,
		bt_solver_, 
		bt_configure_);

	bt_world_->setGravity(btVector3(0, -10, 0));
}
void World::InitScene(void){
	// Ground aka Box
	btTransform ground_transform;
	ground_transform.setIdentity();
	ground_transform.setOrigin(btVector3(0, -56, 0));
	objects_.push_back( new Box(this, nullptr, ground_transform, glm::vec3(50,50,50))  );
	// Sphere
	btTransform start_transform;
	start_transform.setIdentity();
	start_transform.setOrigin(btVector3(2, 10, 0));
	objects_.push_back( new Sphere(this, nullptr, start_transform, 1) );
}
void World::Update(void){ // sync mesh and render
	float last_time, current_time;
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	current_time = glfwGetTime();
	float delta_time = current_time - last_time;
	last_time = current_time;

	bt_world_->stepSimulation(delta_time);
	btTransform transform;

	for(auto& obj: objects_){
		obj->Draw(camera_);
	}

	glfwSwapBuffers(window_);
	glfwPollEvents();
}
void World::Run(void){
	while (!glfwWindowShouldClose(window_)){
		Update();
	}
}
void World::CursorPosCallback(GLFWwindow *window, double x, double y) {
	static double mouse_x, mouse_y;
	static bool entered = false;
	double new_x = x / World::width_ - 0.5;
	double new_y = y / World::height_ - 0.5;
	if (entered) {
		World::camera_->Rotate(mouse_x - new_x, mouse_y - new_y);
	}
	mouse_x = new_x;
	mouse_y = new_y;
	entered = true;
}