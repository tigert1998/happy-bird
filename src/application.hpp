#pragma once

#define GL_GLEXT_PROTOTYPES

#ifdef DEBUG
#include <iostream>
#endif

#include <algorithm>

// Please always use shared to run this program 
class Application {
 private:
	int width = 800, height = 600;
	GLFWwindow* window;

	Camera* camera_ptr;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver*  solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	Object* hero;
	Object* ground;

	static bool keys_pressed[1024];

	static void CursorPosCallback(GLFWwindow *window, double x, double y);
	static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	static void ProcessInput(GLFWwindow *window);
	static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
	static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

 public:
	static Application shared;
	Application();
	void Run();

};

void Application::CursorPosCallback(GLFWwindow *window, double x, double y) {
	static double mouse_x, mouse_y;
	static bool entered = false;
	double new_x = x / shared.width - 0.5;
	double new_y = y / shared.height - 0.5;
	// if (entered) {
	// 	shared.camera_ptr->Rotate(mouse_x - new_x, mouse_y - new_y);
	// 	shared.car_ptr->Rotate(mouse_x - new_x);
	// 	shared.car_ptr->CameraAccompany();
	// }
	mouse_x = new_x;
	mouse_y = new_y;
	entered = true;
}

void Application::FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	shared.width = width;
	shared.height = height;
	shared.camera_ptr->set_width_height_ratio(1.0f * width / height);
}

void Application::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else
    	keys_pressed[key] = action != GLFW_RELEASE;
}

void Application::ProcessInput(GLFWwindow *window) {
	static float current_time, last_time;
	current_time = glfwGetTime();
	float delta_time = current_time - last_time;
	last_time = current_time;
	// if (keys_pressed[GLFW_KEY_W]) 
 //       	shared.car_ptr->Move(MoveDirectionType::FRONT, delta_time);
 //    if (keys_pressed[GLFW_KEY_S])
 //       	shared.car_ptr->Move(MoveDirectionType::BACK, delta_time);
 //    if (keys_pressed[GLFW_KEY_A])
 //       	shared.car_ptr->Move(MoveDirectionType::LEFT, delta_time);
 //    if (keys_pressed[GLFW_KEY_D])
 //       	shared.car_ptr->Move(MoveDirectionType::RIGHT, delta_time);

#ifdef DEBUG
	// if (keys_pressed[GLFW_KEY_SPACE]) {
	// 	shared.car_ptr->ShowPosition();
	// }
#endif
}

Application::Application() {
	using namespace std;
	using namespace glm;
	// opengl init //
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "Anti-vice City", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) cout << "Fail to initial GLAD procedure." <<endl;
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	// bullet3 engine init //
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	// initial object //
	ground = new Box( 50,50,50);
	hero = new Sphere(5);
	ground->Bind(dynamicsWorld);
	hero->Bind(dynamicsWorld);
}

void Application::Run() {
	using namespace glm;
	using namespace std;
	camera_ptr = new Camera(glm::vec3(0, 0, 3), 0, 0, 1.0f * width / height);

	// Shader *skybox_shader_ptr = new Shader("shaders/skybox.vs", "shaders/skybox.fs");
	// skybox_ptr = new Skybox(skybox_urls, *skybox_shader_ptr, *camera_ptr);

	// Model *world_model_ptr = new Model("resources/models/world", "world.obj", false);
	// Shader *world_shader_ptr = new Shader("shaders/world.vs", "shaders/world.fs");
	// world_ptr = new World(*world_model_ptr, *world_shader_ptr, *camera_ptr);

	// Model *car_model_ptr = new Model("resources/models/car", "tank_tigher.obj", true);
	// Shader *car_shader_ptr = new Shader("shaders/car.vs", "shaders/car.fs");
	// car_ptr = new Car(*car_model_ptr, *car_shader_ptr, *camera_ptr, vec3(8.31, 8.01, 4.88));
	Shader* common_shader = new Shader("shaders/common.vs", "shaders/common.fs");

	cout << "Resources all loaded" << endl;

	float last_time = 0.0f, current_time = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dynamicsWorld->stepSimulation(1.0/60, 10);

		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
			printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}

		current_time = glfwGetTime();
		float delta_time = current_time - last_time;
		last_time = current_time;
		// car_ptr->Update(delta_time);


		// if (car_model_ptr->Conflict(*world_model_ptr, car_ptr->model_matrix(), world_ptr->model_matrix())) {
			// car_ptr->Disable();
		// } else {
			// car_ptr->Enable();
		// }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}