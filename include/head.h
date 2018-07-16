#pragma once

#include "bullet_common.h" // btTransform
#include "color.h" // Color, color::Red
#include "object.h" // LivingObject
#include "character.h"
#include "camera.h"
#include "shader.h"
#include "light.h"

class World;
class Cloth;

class Head: public LivingObject{
	friend Cloth;
	float radius_;
 public:
 	Head() = delete;
	Head(World*, Shader*, const btTransform&, float, Color color = color::Red());
	~Head();
	btVector3 GetOrigin(void);
	float radius(void) const;
	void Draw(Camera* camera, const Light* light);
};

