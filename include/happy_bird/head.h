#pragma once

#include "bullet_common.h" // btTransform
#include "color.h" // Color, color::Red
#include "object.h" // LivingObject
#include "character.h"
#include "camera.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"

class World;
class Cloth;

class Head: public LivingObject{
	friend Cloth;
	float radius_;
 public:
 	Head() = delete;
	Head(World* world, Shader* shader, Material* material, const btTransform& transform, float radius);
	~Head();
	btVector3 GetOrigin(void);
	float radius(void) const;
	void Draw(Camera* camera, const LightCollection* light_collection);
};

