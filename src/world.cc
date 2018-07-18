#include <iostream>
using namespace std;

#include "world.h"
#include "object_common.h"
#include "shader_utility/spot_light.h"

int World::height = 600;
int World::width = 800;
bool World::keys_pressed[1024];

Camera* World::camera = new Camera(glm::vec3(25, 51, 25), (double) World::width / (double) World::height);
LightCollection* World::light_collection = new LightCollection(glm::vec3(0, 0, 0));
btVector3 World::origin(0, 0, 0);
btVector3 World::forward(0,0,1);
btVector3 World::left(1,0,0);
btVector3 World::up(0,1,0);
btScalar World::character_height(8);
btScalar World::bounding_height(100);
btScalar World::max_speed(15.0);

World::World() {
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

  btVector3 localInertia(0, 0, 0);
  if (isDynamic)
    shape->calculateLocalInertia(mass,localInertia);

  btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
  
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);

  bt_world_->addRigidBody(body);

  return body;
}

void World::InitGraphics(void) {
  // cout << "[World::InitGraphics()]" << endl;
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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  memset(keys_pressed, 0, sizeof(keys_pressed));
}

void World::InitPhysics(void) {
  // cout << "[World::InitPhysics()]" << endl;
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
  // Ground aka Box
  // cout << "[World::InitScene()]" << endl;
  // btTransform ground_transform;
  // btScalar half_bound = 50;
  // ground_transform.setIdentity();
  // ground_transform.setOrigin(btVector3(0, -half_bound , 0));
  // objects_.push_back(new Box(
  //   this, nullptr, new Material(color::White(), color::White(), 8),
  //   0, ground_transform, 
  //   glm::vec3(half_bound, half_bound, half_bound)
  // ));
  // btTransform box_transform;
  // btScalar box_half = World::character_height;
  // box_transform.setIdentity();
  // box_transform.setOrigin(btVector3(box_half * 2, box_half, 0));
  // objects_.push_back(new Box(
  //   this, nullptr, new Material(color::White(), color::White(), 8),
  //   35, box_transform, glm::vec3(box_half, box_half, box_half)
  // ));
  // Groud aka Wall
  btTransform ground_transform;
  btScalar half_bound = 50;
  float thickness = 5;
  ground_transform.setIdentity();
  ground_transform.setOrigin(btVector3(0, -thickness / 2.0 , 0));
  btMatrix3x3 orn = ground_transform.getBasis();
  orn *= btMatrix3x3(btQuaternion(btVector3(1,0,0), glm::pi<float>()/2.0));
  ground_transform.setBasis(orn);
  objects_.push_back(new Wall(
    this, nullptr, new Material(color::White(), color::White(), 3),
    ground_transform, 
    thickness,
    glm::vec3(half_bound, half_bound, 0)
  ));
  // btTransform box_transform;
  btScalar box_half = World::character_height;
  // box_transform.setIdentity();
  // box_transform.setOrigin(btVector3(box_half * 2, box_half, 0));
  // objects_.push_back(new Box(
  //   this, nullptr, new Material(color::White(), color::White(), 8),
  //   35, box_transform, glm::vec3(box_half, box_half, box_half)
  // )); 
  // Sphere
  btTransform start_transform;
  start_transform.setIdentity();
  start_transform.setOrigin( World::origin + btVector3(0, World::character_height, 0));
  // LivingObject* man = new Head(
  //   this, nullptr, new Material(color::White(), color::White(), 8),
  //   start_transform, World::character_height / 4
  // );
  LivingObject* man = new Hero(
    this, nullptr, new Material(color::White(), color::White(), 8),
    start_transform, World::character_height / 2, World::character_height
  );
  objects_.push_back(man);
  character_ = man->character_;

  start_transform.setOrigin( World::origin + btVector3(0, World::character_height * 2, 0));
  objects_.push_back(new Sphere(this, nullptr, new Material(color::White(), color::White(), 8),
    start_transform, World::character_height / 4));

  camera->set_accompany_object(man, 120);
  // Cloth
  // objects_.push_back(new Cloth(
  //   this, nullptr, new Material(color::White(), color::White(), 8), 
  //   5, 6, 8, dynamic_cast<Head*>(objects_.back())
  // ));
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
      glm::vec3(25, 25, 25), 
      glm::vec3(-1, -1, -1),
      color::White(), 0.8,
      Attenuation(1000),
      glm::pi<float>() / 4
    )
  );
  objects_.push_back( new Particle(this, nullptr, new Material(color::Red(), color::Red(), 20), man));
  float wallHeight = 20;
  float wallWidth = 50;
  btTransform wall_transform;
  wall_transform.setIdentity();
  wall_transform.setOrigin(btVector3( box_half * 2, wallHeight / 2 , box_half));
  objects_.push_back( new Wall(
    this, nullptr, 
    new Material(color::White(), color::White(), 2), 
    wall_transform, 
    thickness, 
    glm::vec3(wallWidth/2, wallHeight/2, 0))
  );
}
void World::Update(void) { // sync mesh and render
  // cout << "[World::Update()]" << endl;
  ProcessInput();
  static float last_time = glfwGetTime(), current_time = 0;
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  current_time = glfwGetTime();
  float delta_time = current_time - last_time;
  last_time = current_time;

  bt_world_->stepSimulation(delta_time);

  for(auto& obj: objects_) {
    obj->Draw(camera, light_collection);
  }
  glfwSwapBuffers(window_);
  glfwPollEvents();
}
void World::Run(void) {
  // cout << "[World::Run()]" << endl;
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
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else{
    keys_pressed[key] = (action != GLFW_RELEASE);
  }
}
void World::ProcessInput(void) {
  if(!character_)return ;
  static float current_time, last_time = glfwGetTime();
  current_time = glfwGetTime();
  float delta_time = (current_time - last_time);
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
  if (keys_pressed[GLFW_KEY_SPACE])
    character_->Jump(delta_time);
}