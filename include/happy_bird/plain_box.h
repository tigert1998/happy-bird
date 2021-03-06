#pragma once

#include "opengl_common.h" // glm
#include "bullet_common.h" // btTransform
#include "object.h" // DeadObject
#include "color.h" // Color, color::Yellow
#include "camera.h"
#include "shader_utility/shader.h"

class World;

class PlainBox: public Object{
	glm::vec3 half_extents_;
	float mass_;
 public:
	PlainBox() = delete;
	PlainBox(World*, Shader*, Material*, const btTransform&, glm::vec3 half, float mass);
	void InitMesh(void);
	void ImportToGraphics(void);
	void Draw(Camera*, const LightCollection*);
	~PlainBox(){ }
};

