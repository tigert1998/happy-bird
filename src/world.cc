#include <iostream>
using namespace std;

#include "world.h"

int World::height_ = 600;
int World::width_ = 800;
bool keys_pressed_[1024];

Camera* World::camera_ = new Camera(glm::vec3(25, 51, 25), (double) World::width_ / (double) World::height_);

World::World(){
  InitPhysics();
  InitGraphics();
  InitScene();
}

World::~World(){
  delete bt_world_;
  delete bt_solver_;
  delete bt_overlapping_paircache_;
  delete bt_dispatcher_;
  delete bt_configure_;
}

btRigidBody* World::createRigidBody (btScalar mass, const btTransform& transform, btCollisionShape* shape)
{
  bool isDynamic = (mass != 0.f);

  btVector3 localInertia(0,0,0);
  if (isDynamic)
    shape->calculateLocalInertia(mass,localInertia);

  btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
  
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

  glfwSetCursorPosCallback(window_, World::CursorPosCallback );
  glfwSetKeyCallback(window_, World::KeyCallback);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  memset(keys_pressed_, 0, sizeof(keys_pressed_));
}

void World::InitPhysics(void){
  bt_configure_ = new btSoftBodyRigidBodyCollisionConfiguration();
  bt_dispatcher_ = new btCollisionDispatcher(bt_configure_);
  bt_overlapping_paircache_ = new btDbvtBroadphase();
  bt_solver_ = new btSequentialImpulseConstraintSolver;
  bt_soft_solver_ = new btDefaultSoftBodySolver;
  bt_world_ = new btSoftRigidDynamicsWorld(bt_dispatcher_,
    bt_overlapping_paircache_,
    bt_solver_, 
    bt_configure_,
    bt_soft_solver_);

  bt_world_->setGravity(btVector3(0, -10, 0));
  bt_soft_info_.m_dispatcher = bt_dispatcher_;
  bt_soft_info_.m_broadphase = bt_overlapping_paircache_;
  bt_soft_info_.m_sparsesdf.Initialize();

  character_ = nullptr;
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
  LivingObject* man = new Head(this, nullptr, start_transform, 2);
  objects_.push_back(man);
  character_ = man->character_;

  camera_->set_accompany_object(man, 10);
  // objects_.push_back( new Sphere(this, nullptr, start_transform, 2) );
  // Cloth
  objects_.push_back( new Cloth(this, nullptr, 5, 6, 8, dynamic_cast<Head*>(objects_.back()) ) );
}
void World::Update(void){ // sync mesh and render
  ProcessInput();
  static float last_time = glfwGetTime(), current_time = 0;
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
void World::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else
    keys_pressed_[key] = (action != GLFW_RELEASE);
}
void World::ProcessInput(void){
  if(!character_)return ;
  static float current_time, last_time;
  current_time = glfwGetTime();
  float delta_time = current_time - last_time;
  last_time = current_time;
  if (keys_pressed_[GLFW_KEY_W]) // Forward
    character_->Move(true, delta_time);
  else if (keys_pressed_[GLFW_KEY_S])
    character_->Move(false, delta_time);
  else
    character_->ResetMove();   
  if (keys_pressed_[GLFW_KEY_A]) // Left
    character_->Rotate(true, delta_time);
  else if (keys_pressed_[GLFW_KEY_D])
    character_->Rotate(false, delta_time);
  if (keys_pressed_[GLFW_KEY_SPACE])
    character_->Jump(delta_time);
}