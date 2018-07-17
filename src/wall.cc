#include "wall.h"

float Wall::margin_ratio_ = 0.15;
float Wall::brick_ratio_ = 2;
std::vector<float> Wall::brick_vertices_ = {
	brick_ratio_ / 2.0f, 0.5f - margin_ratio_, 0.5f - margin_ratio_, // group 1
	brick_ratio_ / 2.0f - margin_ratio_, 0.5f - margin_ratio_, 0.5f,
	brick_ratio_ / 2.0f - margin_ratio_, 0.5f, 0.5f - margin_ratio_,
	brick_ratio_ / 2.0f, 0.5f - margin_ratio_, -(0.5f - margin_ratio_), // group 2
	brick_ratio_ / 2.0f - margin_ratio_, 0.5f - margin_ratio_, -0.5f,
	brick_ratio_ / 2.0f - margin_ratio_, 0.5f, -(0.5f - margin_ratio_),
	brick_ratio_ / 2.0f, -(0.5f - margin_ratio_), 0.5f - margin_ratio_, // group 3
	brick_ratio_ / 2.0f - margin_ratio_, -(0.5f - margin_ratio_), 0.5f,
	brick_ratio_ / 2.0f - margin_ratio_, -0.5f, 0.5f - margin_ratio_,
	-(brick_ratio_ / 2.0f), 0.5f - margin_ratio_, 0.5f - margin_ratio_, // group 4
	-(brick_ratio_ / 2.0f - margin_ratio_), 0.5f - margin_ratio_, 0.5f,
	-(brick_ratio_ / 2.0f - margin_ratio_), 0.5f, 0.5f - margin_ratio_,
	brick_ratio_ / 2.0f, -(0.5f - margin_ratio_), -(0.5f - margin_ratio_), // group 5
	brick_ratio_ / 2.0f - margin_ratio_, -(0.5f - margin_ratio_), -0.5f,
	brick_ratio_ / 2.0f - margin_ratio_, -0.5f, -(0.5f - margin_ratio_),
	-(brick_ratio_ / 2.0f), -(0.5f - margin_ratio_), 0.5f - margin_ratio_, // group 6
	-(brick_ratio_ / 2.0f - margin_ratio_), -(0.5f - margin_ratio_), 0.5f,
	-(brick_ratio_ / 2.0f - margin_ratio_), -0.5f, 0.5f - margin_ratio_,
	-(brick_ratio_ / 2.0f), 0.5f - margin_ratio_, -(0.5f - margin_ratio_), // group 7
	-(brick_ratio_ / 2.0f - margin_ratio_), 0.5f - margin_ratio_, -0.5f,
	-(brick_ratio_ / 2.0f - margin_ratio_), 0.5f, -(0.5f - margin_ratio_),
	-(brick_ratio_ / 2.0f), -(0.5f - margin_ratio_), -(0.5f - margin_ratio_), // group 8
	-(brick_ratio_ / 2.0f - margin_ratio_), -(0.5f - margin_ratio_), -0.5f,
	-(brick_ratio_ / 2.0f - margin_ratio_), -0.5f, -(0.5f - margin_ratio_)
};

Wall::Wall(
	World* world, 
	Shader* shader, 
	const btTransform& trans, 
	float scaling, 
	glm::vec3 half_extents, 
	Color color):
	DeadObject(world,shader), half_extents_(half_extents), scaling_(scaling) {
	assert(world_);
	is_soft_ = false;
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->CreateRigidBody(
		0,
		trans,
		new btBoxShape(GLMVec3ToBTVector3(half_extents))
	);
	// create mesh //
	InitBricks(trans);
	ImportToGraphics();
	// patch shader
	if (!shader) {
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	}
}

void Wall::InitBricks(const btTransform& parent_transform){
	btTransform baseTransform; // point to left-down corner
	btTransform tmpTransform;
	tmpTransform.setIdentity();
	tmpTransform.setOrigin(btVector3(
		scaling_ * brick_ratio_ / 2.0 - half_extents_[0] / 2.0,
		scaling_ / 2.0 - half_extents_[1] / 2.0,
		0
	) );
	baseTransform = tmpTransform * parent_transform;
	int totalLevel = half_extents_[1] / scaling_;
	half_extents_[1] = totalLevel * scaling_;
	half_extents_[2] = scaling_;
	struct Task{
		int level;
		float begin;
		float end;
		Task(int a, float b, float c):level(a),begin(b),end(c){ }
	};
	std::vector<Task> tasks;
	int head = 0;
	int bricks = 0;
	tasks.push_back(Task(0, 0, half_extents_[0]));
	while(head < tasks.size()){
		float begin = tasks[head].begin;
		int level = tasks[head].level;
		float nextBegin = 0;
		while(begin < tasks[head].end){
			btTransform currentTransform;
			currentTransform.setIdentity();
			if(bricks % 7 == 0 ){
				currentTransform.setOrigin(btVector3(
					begin + scaling_ / 2.0,
					level * scaling_ + scaling_ * brick_ratio_ / 2.0, 
					0.0
				));
				btMatrix3x3 orn = currentTransform.getBasis();
				orn *= btMatrix3x3(btQuaternion(btVector3(0,0,1), glm::pi<float>()/2.0));
				currentTransform.setBasis(orn);
				currentTransform = currentTransform * baseTransform;
				tasks.push_back(Task(level + 1, nextBegin, begin));
				begin += scaling_;
				nextBegin = begin;
			}
			else{
				currentTransform.setOrigin(btVector3(
					begin + scaling_ * brick_ratio_ / 2.0,
					level * scaling_ + scaling_ / 2.0, 
					0.0
				));
				currentTransform = currentTransform * baseTransform;
				begin += scaling_ * brick_ratio_;
			}
			for(int v = 0; v < brick_vertices_.size() / 3; v++){
				btVector3 vertice = 
					currentTransform * 
					btVector3(brick_vertices_[v], brick_vertices_[v+1], brick_vertices_[v+2]);
				indices_.push_back(vertices_.size() / 3);
				vertices_.push_back(vertice[0]);
				vertices_.push_back(vertice[1]);
				vertices_.push_back(vertice[2]);
			}
			bricks ++;
		}
		tasks.push_back(Task(level + 1, nextBegin, half_extents_[0]));
		head ++;
	}
	return ;
}
