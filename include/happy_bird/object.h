#pragma once

#include <vector>
#include <string>

#include "color.h"
#include "bullet_common.h" // btCollisionObject
#include "character.h"
#include "math_utility.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"
#include "shader_utility/material.h"

class World;
class Camera;

class Object{
 public:
	// Bullet physical world reference
	World* world_;
	// OpenGL util
	Shader* shader_;
	uint32_t vao_, normal_vbo_, position_vbo_, ebo_;
	// Bullet shape
	bool is_soft_;
	btCollisionObject* bt_object_;
	// object data
	Material* material_;
	std::vector<float> vertices_;
	std::vector<float> normals_;
	std::vector<uint32_t> indices_;

	Object(World* w, Shader* shader, Material* material);
	virtual ~Object();
	virtual void ImportToPhysics();
	virtual void DeleteFromPhysics();
	virtual void ImportToGraphics();
	virtual void Draw(Camera* camera, const btTransform& transform, const LightCollection* light_collection);
	virtual void Draw(Camera* camera, const LightCollection* light_collection);
	virtual void InitMesh(void); // init from shape
	virtual void InitRigidMesh(btCollisionShape* shape, const btTransform& transform);
	virtual void InitMesh(std::string path){ } // init from assimp
	virtual void InitSoftMesh(btSoftBody* shape);
	virtual btVector3 GetOrigin(void); 
	virtual btTransform GetTransform(void);
	inline bool is_soft(void);
	inline void addTriangle(const btVector3& a, const btVector3& b, const btVector3& c);
};

typedef Object DeadObject;
class LivingObject: public Object{
 public:
	Character *character_;
	LivingObject(World* world, Shader* shader, Material* material);
};
