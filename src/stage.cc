#include "stage.h"
#include "opengl_common.h"
#include "bullet_common.h"
#include "object.h"
#include "object_common.h"
#include "shader_utility/material.h"
#include "shader_utility/texture_material.h"
#include "shader_utility/pure_color_material.h"
#include "shader_utility/light.h"
#include "shader_utility/spot_light.h"
#include "shader_utility/parallel_light.h"

void Stage::InitStage(StageFlag flag){
	switch(flag){
		case(kDungeonStage):
		InitDungeonStage();
		break;
		case(kDefaultStage):
		InitDefaultStage();
		break;
	}
}
void Stage::InitDungeonStage(void){
	return ;
}
void Stage::InitDefaultStage(void){
	// Groud aka Wall //
	btTransform ground_transform;
	// Ground Size
	btScalar half_bound = 50;
	float thickness = 5;
	// by translate
	ground_transform.setIdentity();
	ground_transform.setOrigin(btVector3(0, -thickness / 2.0 , 0));
	// rotate to horizon
	btMatrix3x3 orn = ground_transform.getBasis();
	orn *= btMatrix3x3(btQuaternion(btVector3(1, 0, 0), glm::pi<float>() / 2.0));
	ground_transform.setBasis(orn);
	objects_.push_back(
		new Wall(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 3),
			ground_transform,
			thickness,
			glm::vec3(half_bound, half_bound, 0)
		)
	);
	// Wall aka Wall //
	// Wall Size
	btScalar box_half = World::character_height;
	float wallHeight = 20;
	float wallWidth = 50;
	// translate
	btTransform wall_transform;
	wall_transform.setIdentity();
	wall_transform.setOrigin(btVector3(box_half * 2, wallHeight / 2 , box_half));
	objects_.push_back(
		new Wall(
			world_, 
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 2),
			wall_transform,
			thickness,
			glm::vec3(wallWidth / 2, wallHeight / 2, 0)
		)
	);

	// Item aka Wall //
	btTransform box_transform;
	box_transform.setIdentity();
	box_transform.setOrigin(World::origin + btVector3(box_half * 2, box_half, 0));
	objects_.push_back(
	 	new Box(
		 	world_, 
		 	nullptr, 
		 	new PureColorMaterial(color::White(), color::White(), 8),
		 	box_transform, 
		 	glm::vec3(box_half, box_half, box_half),
		 	35.0f // mass
		)
	); 
	

}
