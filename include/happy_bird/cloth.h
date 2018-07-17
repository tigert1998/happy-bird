#pragma once

#include "bullet_common.h" // btCollisionObject
#include "object.h" // DeadObject
#include "color.h" // Color, color:::Green
#include "camera.h"
#include "head.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"

class World;

// Attached Object: physicsShape->mesh(mutable)->graphics
class Cloth: public DeadObject{
	float width_;
	float length_;
	int subdivide_;
	btCollisionObject* attached_;
	
 public:
	Cloth() = delete;
	Cloth(
		World* world, 
		Shader* shader, 
		Material* material,
		float attachWid, 
		float clothLen, 
		uint32_t subdivide, 
		Head* head
	);
	void ImportToGraphics();
	~Cloth();
	void Draw(Camera* camera, const LightCollection* light_collection);
};