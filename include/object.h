#pragma once

#include <vector>
#include <string>

#include "color.h"
#include "bullet_common.h" // btCollisionObject
#include "character.h"
#include "shader.h"
#include "vector_utility.h"
#include "light.h"

class World;
class Camera;

class Object{
 public:
	// Bullet physical world reference
	World* world_;
	// OpenGL util
	Shader* shader_;
	uint32_t vao_, vbos_[2], ebo_;
	// Bullet shape
	bool is_soft_;
	btCollisionObject* bt_object_;
	// object data
	Color color_;
	std::vector<float> vertices_;
	std::vector<float> normals_;
	std::vector<uint32_t> indices_;

	Object(World* w, Shader* shader);
	virtual ~Object();
	virtual void ImportToPhysics();
	virtual void DeleteFromPhysics();
	virtual void ImportToGraphics();
	virtual void Draw(Camera* camera, const btTransform& transform, const Light* light);
	virtual void Draw(Camera* camera, const Light* light);
	virtual void InitMesh(void); // init from shape
	virtual void InitRigidMesh(btCollisionShape* shape, const btTransform& transform);
	virtual void InitMesh(std::string path){ } // init from assimp
	virtual void InitSoftMesh(btSoftBody* shape);
	virtual btVector3 GetOrigin(void); 
	// virtual void InitSoftMesh(btCollisionShape* shape);
	inline bool isSoft(void);
	inline void addTriangle(const btVector3& a, const btVector3& b, const btVector3& c);
};

typedef Object DeadObject;
class LivingObject: public Object{
 public:
	Character *character_;
	LivingObject(World* world, Shader* shader);
};

