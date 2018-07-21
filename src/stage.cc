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
	// Ground aka Wall //
	btScalar thickness = 5;
	btScalar depth = 40;
	btScalar width = 270;
	btTransform transGround;
	transGround.setIdentity();
	transGround.setOrigin(btVector3( width / 2.0, -thickness / 2.0, depth / 2.0)); // place in positive quadrant
	// transGround.setOrigin(btVector3(0, -thickness / 2.0 , 0));
	btMatrix3x3 orn = transGround.getBasis(); 
	orn *= btMatrix3x3(btQuaternion(btVector3(1, 0, 0), glm::pi<float>() / 2.0) );
	transGround.setBasis(orn);
	objects_.push_back(
		StageWrapper(std::make_shared<Wall>(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 3),
			transGround,
			thickness,
			glm::vec3(width / 2, depth / 2.0, 0)
		))
	);
	// Main Wall //
	btScalar height = 45;
	btTransform transWall;
	transWall.setIdentity();
	transWall.setOrigin(btVector3(width / 2.0, height / 2.0, depth - thickness / 2.0));
	objects_.push_back(
		StageWrapper(std::make_shared<Wall>(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 2),
			transWall,
			thickness,
			glm::vec3(width / 2.0, height / 2.0, 0)
		))
	);
	// Platform //
	btScalar platformHeight = 35;
	btScalar platformWidth = 40;
	btScalar platformOffset = 40;
	btScalar platformProtrude = 12;
	btTransform transPlatform;
	transPlatform.setIdentity();
	transPlatform.setOrigin(btVector3(platformOffset + platformWidth / 2.0, platformHeight, depth - platformProtrude / 2.0 - thickness / 2.0));
	orn = transPlatform.getBasis() * btMatrix3x3(btQuaternion(btVector3(1,0,0), glm::pi<float>() / 2.0));
	transPlatform.setBasis(orn);
	objects_.push_back(
		StageWrapper(std::make_shared<Wall>(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 2),
			transPlatform,
			thickness,
			glm::vec3(platformWidth / 2.0, platformProtrude / 2.0, 0)
		))
	);
	// Stair
	int firstLevelStair = 3;
	btScalar stairWidth = platformOffset / (firstLevelStair + 1);
	btScalar stairDeltaHeight = platformHeight / 2.0 / (firstLevelStair + 1);
	btTransform transStair;
	transStair = transPlatform;
	for(int i = 0; i < firstLevelStair; i++){		
		transStair.setOrigin(btVector3(platformOffset - stairWidth * i - stairWidth / 2.0, platformHeight - stairDeltaHeight * (i+1), depth - platformProtrude / 2.0 - thickness / 2.0 ));
		objects_.push_back(
			StageWrapper(std::make_shared<Wall>(
				world_,
				nullptr,
				new PureColorMaterial(color::White(), color::White(), 2),
				transStair,
				thickness,
				glm::vec3(stairWidth / 2.0, platformProtrude / 2.0, 0)
			))
		);
	}
	btScalar stairProtrude = 1.8 * platformProtrude;
	transStair.setOrigin(btVector3(stairWidth / 2.0, platformHeight / 2.0, depth - stairProtrude / 2.0 - thickness / 2.0 ));
	objects_.push_back(
		StageWrapper(std::make_shared<Wall>(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 2),
			transStair,
			thickness,
			glm::vec3(stairWidth / 2.0, stairProtrude / 2.0, 0)
		))
	);
	int secondLevelStair = 2;
	stairDeltaHeight = platformHeight / 2.0 / (secondLevelStair + 1);
	for(int i = 1; i <= secondLevelStair; i++){
		transStair.setOrigin(btVector3(stairWidth / 2.0 + i * stairWidth, platformHeight / 2.0 - stairDeltaHeight * i, depth - stairProtrude / 2.0 - thickness / 2.0));
		objects_.push_back(
			StageWrapper(std::make_shared<Wall>(
				world_,
				nullptr,
				new PureColorMaterial(color::White(), color::White(), 2),
				transStair,
				thickness,
				glm::vec3(stairWidth / 2.0, stairProtrude / 2.0, 0)
			))
		);
	}

	// Flame Pillar //
	btScalar pillarHeight = height - 10;
	btScalar pillarWidth = 15;
	btTransform transPillar;
	transPillar.setIdentity();
	transPillar.setOrigin(btVector3(-pillarWidth / 2.0, pillarHeight / 2.0, depth - stairProtrude / 2.0 - pillarWidth / 2.0));
	objects_.push_back(
		StageWrapper(std::make_shared<Wall>(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 2),
			transPillar,
			pillarWidth,
			glm::vec3(pillarWidth / 2.0 + 1, pillarHeight / 2.0 + 1, 0)
		))
	);
	// Flame //
	btVector3 posFlame(-pillarWidth / 2.0, pillarHeight, depth - stairProtrude / 2.0 - pillarWidth / 2.0);
	objects_.push_back(
		StageWrapper(std::make_shared<Particle>(
			world_,
			new Shader("shader/particle.vert", "shader/flame.frag"),
			new PureColorMaterial(color::Red(), color::Red(), 2),
			posFlame,
			glm::vec3(0, 0.007, 0),
			kLargeParticle | kFlameParticle | kAmbientParticle | kJitterParticle | kMockFlame,
			20,
			0.08f
		))
	);
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
		StageWrapper(std::make_shared<Wall>(
			world_,
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 3),
			ground_transform,
			thickness,
			glm::vec3(half_bound, half_bound, 0)
		))
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
		StageWrapper(std::make_shared<Wall>(
			world_, 
			nullptr,
			new PureColorMaterial(color::White(), color::White(), 2),
			wall_transform,
			thickness,
			glm::vec3(wallWidth / 2, wallHeight / 2, 0)
		))
	);

	// Item aka Box //
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
