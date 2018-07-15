#pragma once
#include <vector>
#include <string>

#include "opengl_common.h"
#include "bullet_common.h"
#include "color.h"
#include "shader.h"
#include "camera.h"

class World;

class Object{
 public:
	// Bullet physical world reference
	World* world_;
	// OpenGL util
	Shader* shader_;
	uint32_t vao_, vbo_, ebo_;
	// Bullet shape
	bool is_soft_;
	btCollisionObject* bt_object_;
	// objet data
	Color color_;
	std::vector<float> vertices_;
	std::vector<float> normals_;
	std::vector<uint32_t> indices_;

	Object(World* w, Shader* shader);
	virtual ~Object();
	virtual void ImportToPhysics();
	virtual void DeleteFromPhysics();
	virtual void ImportToGraphics();
	virtual void Draw(Camera* camera, const btTransform& transform);
	virtual void Draw(Camera* camera);
	virtual void InitMesh(void); // init from shape
	virtual void InitRigidMesh(btCollisionShape* shape, const btTransform& trans);
	virtual void InitMesh(std::string path){ } // init from assimp
	virtual void InitSoftMesh(btSoftBody* shape);
	// virtual void InitSoftMesh(btCollisionShape* shape);
	inline bool isSoft(void){return is_soft_;}
	inline void addTriangle(const btVector3& a, const btVector3& b, const btVector3& c){
		indices_.push_back(vertices_.size() / 3);
		indices_.push_back(vertices_.size() / 3 + 1);
		indices_.push_back(vertices_.size() / 3 + 2);
		vertices_.push_back(a[0]);
		vertices_.push_back(a[1]);
		vertices_.push_back(a[2]);
		vertices_.push_back(b[0]);
		vertices_.push_back(b[1]);
		vertices_.push_back(b[2]);
		vertices_.push_back(c[0]);
		vertices_.push_back(c[1]);
		vertices_.push_back(c[2]);
	}
};

typedef Object DeadObject;
// class LivingObject: public Object{
// 	struct Character{
// 		btPairCachingGhostObject* ghost_delegate;
// 		btKinematicCharacterController* controller;
// 	} *character_;
// 	LivingObject(World* world, Shader* shader, Character* character);
// 	virtual void InitControl();
// };


// Plain Object: physicsShape->mesh(immutable),transform->graphics 
class Box: public DeadObject{
	glm::vec3 half_extents_;
 public:
	Box() = delete;
	Box(World* world, Shader* shader, const btTransform& trans, glm::vec3 half_extents, Color color = color::Yellow());
	void Draw(Camera* camera);
	~Box(){ }
};
class Sphere: public DeadObject{
	float radius_;
 public:
	Sphere() = delete;
	Sphere(World* world, Shader* shader, const btTransform& trans, float radius, Color color = color::Red());
	void Draw(Camera* camera);
	~Sphere(){ }
	// accessor
	inline float getRadius(void){return radius_;}
};

// Complex Model: assimp->mesh->boundingBox->physics
// class Model: public DeadObject{

// };


// Plain Object with control
typedef Sphere Head;
// class Head: public LivingObject{
// 	Head(World* world, Shader* shader, Character* character, float radius, Color color = color::Red());
// 	~Head(){ }
// };


// Attached Object: physicsShape->mesh(mutable)->graphics
class Cloth: public DeadObject{
	float width_;
	float length_;
	int subdivide_;
 public:
	Cloth() = delete;
	Cloth(World* world, Shader* shader, float attachWid, float clothLen, uint32_t subdivide, Head* head );
	~Cloth(){ }
	void Draw(Camera*);
};

