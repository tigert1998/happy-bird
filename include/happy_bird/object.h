#pragma once

#include <vector>
#include <string>

#include "color.h"
#include "bullet_common.h" // btCollisionObject
#include "math_utility.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"
#include "shader_utility/material.h"

class World;
class Camera;
class Character;
class CharacterImpl;

class Object{
	friend World;
	friend Character;
 protected:
	// Bullet physical world reference
	World* world_;
	// OpenGL util
	Shader* shader_;
	uint32_t stride_;
	uint32_t vao_;
	uint32_t vbo_;
	uint32_t ebo_;
	// Bullet shape
	bool is_soft_;
	btCollisionObject* bt_object_;
	// object data
	Material* material_;
	std::vector<float> data_;
	std::vector<uint32_t> indices_;
	// external util
	Object* anchor_;
	btVector3 offset_;
 private:
 	// for recursive call
 	void InitRigidMesh(btCollisionShape* shape, const btTransform& transform);
 	void AddTriangle(const btVector3& a, const btVector3& b, const btVector3& c);
 protected:
	// Shared Method
	void InitRigidMesh(void); // takes up three holder
	void InitSoftMesh(void); // takes up six holder
 public:
	Object(World* w, Shader* shader, Material* material, uint32_t stride, bool is_soft = false);
	virtual ~Object();
	// Public Interface:
	virtual void Draw(Camera* camera, const LightCollection* light_collection); // have default impl
	// accessor
	btVector3 GetOrigin(void);
	btTransform GetTransform(void);
	btCollisionObject* GetBulletObject(void);
	// for attach
	void Attach(Object* target, const btVector3& offset); // delete from bullet
	// for controller
	void ActivateControl(void); // activate bullet set
	btVector3 GetVelocity(void);
	void SetVelocity(const btVector3&);
};
