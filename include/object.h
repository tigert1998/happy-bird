#pragma once
#include <vector>

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
	btCollisionObject* bt_object_;
	// objet data
	Color color_;
	std::vector<float> vertices_;
	std::vector<float> normals_;
	std::vector<uint32_t> indices_;

	Object(World* w, Shader* shader):
		world_(w),
		shader_(shader),
		bt_object_(nullptr){ }
	virtual ~Object(){
		// delete world_;
		delete shader_;
		delete bt_object_;
	}
	virtual void ImportToPhysics();
	virtual void DeleteFromPhysics();
	virtual void ImportToGraphics();
	virtual void Draw(Camera* camera, const btTransform& transform);
	virtual void Draw(Camera* camera);
	virtual void InitMesh(void); // init from shape
	virtual void InitMesh(btCollisionShape* shape, const btTransform& trans);
	virtual void InitMesh(std::string path){ } // init from assimp
};

typedef Object DeadObject;
// class LivingObject: public Object{
// 	struct Character{
// 		btPairCachingGhostObject* ghost_delegate;
// 		btKinematicCharacterController* controller;
// 	} *character_;
// 	LivingObject(Character* character):character_(character){ }
// 	virtual void InitControl(){

// 	}
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
};

// Complex Model: assimp->mesh->boundingBox->physics
// class Model: public DeadObject{

// };


// Plain Object with control
// class Head: public LivingObject{
// 	Head(World* world, Shader* shader, Character* character, float radius, Color color = color::Red());

// };
// class Arrow: public LivingObject{
// 	Arrow() = delete;
// 	Arrow(Character* character, float len, Color color):
// 		LivingObject(character),length_(len),color_(color){ }
// };

// Attached Object: physicsShape->mesh(mutable)->graphics
// class Cloth: public DeadObject{
// 	Cloth() = delete;
// 	Cloth(World* world, Shader* shader, float attachWid, float clothLen, uint32_t subdivide, Head* head );
// 	~Cloth(){ }
// };
// class Particles: public DeadObject{
// 	Particles() = delete;
// 	Particles(Arrow* arrow, Color color);
// };
// class Leaf: public DeadObject{
// 	Leaf() = delete;
// 	Leaf(float length);
// 	~Leaf(){ }
// };


struct	btSoftBodyWorldInfo
{
	btScalar				air_density;
	btScalar				water_density;
	btScalar				water_offset;
	btScalar				m_maxDisplacement;
	btVector3				water_normal;
	btBroadphaseInterface*	m_broadphase;
	btDispatcher*	m_dispatcher;
	btVector3				m_gravity;
	btSparseSdf<3>			m_sparsesdf;

	btSoftBodyWorldInfo()
		:air_density((btScalar)1.2),
		water_density(0),
		water_offset(0),
		m_maxDisplacement(1000.f),//avoid soft body from 'exploding' so use some upper threshold of maximum motion that a node can travel per frame
		water_normal(0,0,0),
		m_broadphase(0),
		m_dispatcher(0),
		m_gravity(0,-10,0)
	{
	}
};	
