#pragma once

#include "bullet_common.h" // btTransform
#include "color.h" // Color, color::Red
#include "object.h" // LivingObject
#include "character.h"
#include "camera.h"
#include "shader_utility/shader.h"
#include "shader_utility/light_collection.h"

#include <vector>

class World;

class Hero: public LivingObject{
private:
	float width_;
	float height_;
	static float thickness_;
	uint32_t tex_coord_vbo_;
	std::vector<float> tex_coords_;

public:
 	Hero() = delete;
	Hero(World* world, Shader* shader, Material* material, const btTransform& transform, float w, float h);
	~Hero();
	btVector3 GetOrigin(void);
	float height(void) const;
	float width(void) const;
	void Draw(Camera* camera, const LightCollection* light_collection);
	void ImportToGraphics();
};
