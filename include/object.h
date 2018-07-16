#pragma once
#include <vector>
#include <string>

#include "opengl_common.h"
#include "bullet_common.h"
#include "color.h"

class World;
class Camera;
class Shader;
class Character;

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
class LivingObject: public Object{
 public:
	Character *character_;
	LivingObject(World* world, Shader* shader);
};

