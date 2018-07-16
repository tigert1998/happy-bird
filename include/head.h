#pragma once

#include "color.h"
#include "object.h"

class World;
class Character;
class Camera;
class Shader;

class Head: public LivingObject{
	float radius_;
 public:
	btRigidBody* delegate_;
 	Head() = delete;
	Head(World*, Shader*, const btTransform&, float, Color color = color::Red());
	~Head();
	void Draw(Camera* camera, const Light* light);
	float getRadius(void) const;
};

