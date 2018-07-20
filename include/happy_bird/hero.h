#pragma once

#include "bullet_common.h" // btTransform
#include "color.h" // Color, color::Red
#include "object.h" // LivingObject
#include "camera.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"
#include "controller_utility/character_impl.h"

#include <vector>

class World;

class Hero: public Object {
private:
	float width_;
	float height_;
	static float thickness_;
public:
 	Hero() = delete;
	Hero(World* world, Shader* shader, Material* material, const btTransform& transform, float w, float h);
	~Hero();
	btVector3 GetOrigin();
	btTransform GetTransform();
	float height() const;
	float width() const;
	void ImportToGraphics();
	void InitMesh();
};
