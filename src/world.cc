#include <iostream>
using namespace std;

#include "world.h"
#include "object_common.h"
#include "shader_utility/spot_light.h"
#include "shader_utility/pure_color_material.h"
#include "shader_utility/texture_material.h"
#include "random.h"
int World::height = 600;
int World::width = 800;
bool World::keys_pressed[1024];
bool World::exit = false;

Camera* World::camera = new Camera(glm::vec3(25, 51, 25), (double) World::width / (double) World::height);
LightCollection* World::light_collection = new LightCollection(glm::vec3(0, 0, 0));
btVector3 World::origin(0, 0, 0);
btVector3 World::forward(0, 0, 1);
btVector3 World::left(1, 0, 0);
btVector3 World::up(0, 1, 0);
btScalar World::character_height(8);
btScalar World::bounding_height(100);

World::World(): stage_(this), temp_(this) {
	InitPhysics();
	InitGraphics();
	InitScene();
}

World::~World() {
	delete bt_world_;
	delete bt_solver_;
	delete bt_overlapping_paircache_;
	delete bt_dispatcher_;
	delete bt_configure_;
}

btSoftRigidDynamicsWorld* World::bt_world(void) const {
	return bt_world_;
}

btBroadphaseInterface* World::bt_broadphase(void) const{
	return bt_overlapping_paircache_;
}

btSoftBodyWorldInfo& World::bt_info() {
	return bt_soft_info_;
}

btRigidBody* World::CreateRigidBody(btScalar mass, const btTransform& transform, btCollisionShape* shape) {
	bool isDynamic = (mass != 0.f);

	btVector3 local_inertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, local_inertia);

	btDefaultMotionState* my_motion_state = new btDefaultMotionState(transform);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, my_motion_state, shape, local_inertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	bt_world_->addRigidBody(body);

	return body;
}

void World::InitGraphics(void) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window_ = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window_);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

	glfwSetCursorPosCallback(window_, World::CursorPosCallback );
	glfwSetKeyCallback(window_, World::KeyCallback);
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	// for sprite
	glEnable(GL_POINT_SPRITE_ARB);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	// for alpha texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	memset(keys_pressed, 0, sizeof(keys_pressed));
}

void World::InitPhysics(void) {
	bt_configure_ = new btSoftBodyRigidBodyCollisionConfiguration();
	bt_dispatcher_ = new btCollisionDispatcher(bt_configure_);
	bt_overlapping_paircache_ = new btDbvtBroadphase();
	bt_solver_ = new btSequentialImpulseConstraintSolver;
	bt_soft_solver_ = new btDefaultSoftBodySolver;
	bt_world_ = new btSoftRigidDynamicsWorld(
		bt_dispatcher_,
		bt_overlapping_paircache_,
		bt_solver_, 
		bt_configure_,
		bt_soft_solver_
	);

	bt_world_->setGravity(btVector3(0, -50, 0));
	bt_soft_info_.m_dispatcher = bt_dispatcher_;
	bt_soft_info_.m_broadphase = bt_overlapping_paircache_;
	bt_soft_info_.m_sparsesdf.Initialize();

	character_ = nullptr;
}

void World::InitScene(void) {
	// stage_.InitStage(kDefaultStage);
	stage_.InitStage(kDungeonStage);
	cout << "After InitStage" << endl;

	btTransform start_transform;
	start_transform.setIdentity();
	start_transform.setOrigin( World::origin + btVector3(45, 40, 30));
	// start_transform.setOrigin( World::origin + btVector3(5, World::character_height, 5));
	Object* man = new Hero(
		this,
		nullptr,
		new TextureMaterial("resources/hero.png", "resources/hero.png", 8),
		start_transform,
		World::character_height,
		World::character_height
	);
	objects_.push_back(man);
	character_ = new Character(this, man);
	// objects_.push_back( 
	// 	new Sphere(
	// 		this, 
	// 		nullptr, 
	// 		new PureColorMaterial(color::White(), color::White(), 3),
	// 		start_transform,
	// 		3
	// 	)
	// );
	// objects_.back()->Attach(man, btVector3(0,3,0));

	camera->set_accompany_object(man, 120);
	// objects_.push_back(
	// 	new Particle(
	// 		this,
	// 		new Shader("shader/particle.vert", "shader/blood_incr.frag"),
	// 		new PureColorMaterial(color::Green(), color::Green(), 40),
	// 		btVector3(0, 0, 0), // position
	// 		glm::vec3(0, 0, -0.02),
	// 		kMediumParticle | kFlameParticle | kAmbientParticle | kJitterParticle
	// 	)
	// );
	// objects_.back()->Attach(objects_[objects_.size()-2], btVector3(0,3,0));

	light_collection->PushBack(
		new PointLight(
			glm::vec3(0, 15, 0),
			glm::vec3(0, 1, 0), 0.6, 
			Attenuation(1200)
		)
	);
	light_collection->PushBack(
		new ParallelLight(
			glm::vec3(-1, -1, -1), 
			glm::vec3(1, 0, 0), 0.1
		)
	);
	light_collection->PushBack(
		new SpotLight(
			glm::vec3(0, 0, 0), 
			glm::vec3(10, 5, 5),
			color::White(), 
			10,
			Attenuation(1800),
			glm::pi<float>() / 4
		)
	);
}

void World::Update(void) { // sync mesh and render
	ProcessInput();
	static float last_time = glfwGetTime(), current_time = 0;
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	current_time = glfwGetTime();
	float delta_time = current_time - last_time;
	last_time = current_time;

	bt_world_->stepSimulation(delta_time);

	if(Random::QueryIntRandom(0, 2000) < 2)	
		temp_.PushBack(
			new Particle(
				this,
				nullptr,
				new PureColorMaterial(color::Red(), color::Red(), 40),
				btVector3(Random::QueryFloatRandom(-50, 50), Random::QueryFloatRandom(-50, 50), Random::QueryFloatRandom(-50, 50)),
				glm::vec3(0, 0, -0.02),
				kLargeParticle | kFlameParticle | kAmbientParticle,
				8,
				0.07,
				4
			),
			6
		);
	temp_.Update();	
	Timer::UpdateFrame();
	// Drawing
	for(auto p = stage_.begin(); p != stage_.end(); p++){
		auto ptr = p->get().lock();
		if(ptr){
			ptr->Draw(camera, light_collection);
		}
	}
	for(auto p = temp_.begin(); p != temp_.end(); p++){
		auto ptr = p->get().lock();
		if(ptr){
			ptr->Draw(camera, light_collection);
		}
	}
	for(auto& obj : objects_) {
		obj->Draw(camera, light_collection);
	}
	glfwSwapBuffers(window_);
	glfwPollEvents();
}

void World::Run(void) {
	while (!glfwWindowShouldClose(window_)) {
		Update();
	}
}

void World::CursorPosCallback(GLFWwindow *window, double x, double y) {
	static double mouse_x, mouse_y;
	static bool entered = false;
	double new_x = x / World::width - 0.5;
	double new_y = y / World::height - 0.5;
	if (entered) {
		World::camera->Rotate(mouse_x - new_x, mouse_y - new_y);
	}
	mouse_x = new_x;
	mouse_y = new_y;
	entered = true;
}

void World::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		World::exit = true;
		glfwSetWindowShouldClose(window, GL_TRUE);
	} else {
		keys_pressed[key] = (action != GLFW_RELEASE);
	}
}

void World::ProcessInput(void) {
	if (!character_) return;
	static float current_time, last_time = glfwGetTime();
	current_time = glfwGetTime();
	float delta_time = current_time - last_time;
	last_time = current_time;
	if (keys_pressed[GLFW_KEY_W]) // Forward
		character_->Move(true, delta_time);
	else if (keys_pressed[GLFW_KEY_S])
		character_->Move(false, delta_time);
	else
		character_->ResetMove();
	if (keys_pressed[GLFW_KEY_A]) // Left
		character_->Rotate(true, delta_time);
	else if (keys_pressed[GLFW_KEY_D])
		character_->Rotate(false, delta_time);
	else
		character_->ResetRotate();

	if(keys_pressed[GLFW_KEY_F]){
		character_->LaserAttack();
		character_->Gain(10);
	}
	if(keys_pressed[GLFW_KEY_G]){
		character_->BoxAttack();
		character_->Lose(10);
	}

	if (keys_pressed[GLFW_KEY_SPACE]) {
		character_->Jump(delta_time);
	#if defined(ENABLE_AUDIO) && (defined (__WIN32) || defined (__WIN64) || defined (_MSC_VER))
		PlaySound(TEXT("./audio/sounds/ding.wav"), NULL, SND_ASYNC | SND_FILENAME);
	#endif
	}
		
}