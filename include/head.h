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
 	Head() = delete;
	Head(World*, Shader*, const btTransform&, float, Color color = color::Red());
	~Head(){ }
	void Draw(Camera*);
	inline float getRadius(void) const {return radius_;}
};

