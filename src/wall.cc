#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

#include "wall.h"

float Wall::margin_ratio_ = 0.1;
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
std::vector<float> Wall::brick_normals_ = {
	0.8547634353587378f, 0.3670010010706571f, 0.3670010010706571f, // 1
	0.3670010010706571f, 0.3670010010706571f, 0.8547634353587378f,
	0.3670010010706571f, 0.8547634353587378f, 0.3670010010706571f,
	0.8547634353587378f, 0.3670010010706571f, -0.3670010010706571f, // 2
	0.3670010010706571f, 0.3670010010706571f, -0.8547634353587378f,
	0.3670010010706571f, 0.8547634353587378f, -0.3670010010706571f,
	0.8547634353587378f, -0.3670010010706571f, 0.3670010010706571f, // 3
	0.3670010010706571f, -0.3670010010706571f, 0.8547634353587378f,
	0.3670010010706571f, -0.8547634353587378f, 0.3670010010706571f,
	-0.8547634353587378f, 0.3670010010706571f, 0.3670010010706571f, // 4
	-0.3670010010706571f, 0.3670010010706571f, 0.8547634353587378f,
	-0.3670010010706571f, 0.8547634353587378f, 0.3670010010706571f,
	0.8547634353587378f, -0.3670010010706571f, -0.3670010010706571f, // 5
	0.3670010010706571f, -0.3670010010706571f, -0.8547634353587378f,
	0.3670010010706571f, -0.8547634353587378f, -0.3670010010706571f,
	-0.8547634353587378f, -0.3670010010706571f, 0.3670010010706571f, // 6
	-0.3670010010706571f, -0.3670010010706571f, 0.8547634353587378f,
	-0.3670010010706571f, -0.8547634353587378f, 0.3670010010706571f,
	-0.8547634353587378f, 0.3670010010706571f, -0.3670010010706571f, // 7
	-0.3670010010706571f, 0.3670010010706571f, -0.8547634353587378f,
	-0.3670010010706571f, 0.8547634353587378f, -0.3670010010706571f,
	-0.8547634353587378f, -0.3670010010706571f, -0.3670010010706571f, // 8
	-0.3670010010706571f, -0.3670010010706571f, -0.8547634353587378f,
	-0.3670010010706571f, -0.8547634353587378f, -0.3670010010706571f,
};
std::vector<uint32_t> Wall::brick_indices_ = {
	0, 1, 2, // corner face
	3, 4, 5,
	6, 7, 8,
	9, 10, 11,
	12, 13, 14,
	15, 16, 17,
	18, 19, 20, 
	21, 22, 23,
	0, 1, 6, // skew face
	6, 7, 1,
	1, 2, 10,
	10, 11, 2,
	0, 2, 3,
	3, 5, 2,
	6, 8, 14,
	12, 14, 6,
	7, 8, 16, 
	16, 17, 8,
	15, 17, 21,
	21, 23, 17,
	21, 22, 18,
	18, 22, 19,
	22, 23, 13, 
	13, 14, 23,
	9, 11, 20,
	9, 20, 18,
	3, 4, 12,
	4, 12, 13,
	15, 16, 9,
	9, 10, 16,
	4, 5, 19,
	19, 20, 5,
	2, 5, 11, // face
	5, 11, 20,
	1, 7, 10,
	7, 10, 16,
	0, 3, 6,
	3, 6, 12,
	23, 14, 8,
	23, 8, 17,
	18, 9, 21,
	9, 21, 15,
	19, 4, 13,
	19, 13, 22
};

ostream& operator<<(ostream& os, const btTransform& transform){
	os << "translate = "<< transform.getOrigin()[0] <<", "<< transform.getOrigin()[1] << ", " << transform.getOrigin()[2] << endl;
	return os;
}

Wall::Wall(
	World* world, 
	Shader* shader,
	Material* material,
	const btTransform& trans, 
	float scaling,
	glm::vec3 half_extents
	):
	DeadObject(world, shader, material), 
	half_extents_(half_extents), 
	scaling_(scaling) {
	assert(world_);
	is_soft_ = false;
	if (!shader)
		shader_ = new Shader("shader/common.vert", "shader/common.frag");
	// initialize physics shape //
	half_extents_[2] = scaling_ / 2;
	bt_object_ = world_->CreateRigidBody(
		0,
		trans,
		new btBoxShape(GLMVec3ToBTVector3(half_extents_))
	);
	// create mesh //
	InitBricks(trans);
	ImportToGraphics();

}
#define kErr (0.1)
void Wall::InitBricks(const btTransform& parent_transform){
	btTransform baseTransform; // point to left-down corner
	baseTransform.setIdentity();
	baseTransform.setOrigin(btVector3(
		- half_extents_[0],
		- half_extents_[1],
		0
	) );
	int totalLevel = half_extents_[1] * 2 / scaling_;
	half_extents_[1] = totalLevel * scaling_ / 2.0;

	std::vector<float> flipBarrier[2]; // use level%2 as current barrier, next as forward split
	flipBarrier[0].push_back(half_extents_[0] * 2);
	int bricks = 0;
	int level = 0;
	float current;
	float end;
	while(level < totalLevel){
		end = -scaling_; // initial value
		flipBarrier[(level+1)%2].clear();
		for(int i = 0; i < flipBarrier[level%2].size(); i++){ // init this level
			current = end + scaling_;
			end = flipBarrier[level%2][i];
			while(current + scaling_ < end + kErr){ // init this partition
				btTransform currentTransform;
				currentTransform.setIdentity();
				bool rotated = false;
				if( level + 1 < totalLevel && ((bricks % 3 == 0 && bricks % (level+1) != 0) || (current + scaling_ * brick_ratio_ > half_extents_[0] * 2 + kErr)) ){
					currentTransform.setOrigin(btVector3(
						current + scaling_ / 2.0,
						level * scaling_ + scaling_ * brick_ratio_ / 2.0, 
						0.0
					));
					currentTransform = currentTransform * baseTransform;
					btMatrix3x3 orn = currentTransform.getBasis();
					orn *= btMatrix3x3(btQuaternion(btVector3(0,0,1), glm::pi<float>()/2.0));
					currentTransform.setBasis(orn);
					rotated = true;
					flipBarrier[(level+1)%2].push_back(current);
					current += scaling_;
				}
				else if ((current + scaling_ * brick_ratio_ <= end + kErr)){
					currentTransform.setOrigin(btVector3(
						current + scaling_ * brick_ratio_ / 2.0,
						level * scaling_ + scaling_ / 2.0, 
						0.0
					));
					currentTransform = currentTransform * baseTransform;
					current += scaling_ * brick_ratio_;
				}
				else{
					current += scaling_ * brick_ratio_;
					continue;
				}
				int base = vertices_.size() / 3;
				for(int v = 0; v + 2 < brick_vertices_.size(); v += 3){
					btVector3 vertice = 
						currentTransform * 
						btVector3(brick_vertices_[v] * scaling_, brick_vertices_[v+1] * scaling_, brick_vertices_[v+2] * scaling_);
					vertices_.push_back(vertice[0]);
					vertices_.push_back(vertice[1]);
					vertices_.push_back(vertice[2]);
					// only rotate normal
					if(rotated){
						normals_.push_back(-brick_normals_[v+1]);
						normals_.push_back(brick_normals_[v+0]);
						normals_.push_back(brick_normals_[v+2]);						
					}
					else{
						normals_.push_back(brick_normals_[v+0]);
						normals_.push_back(brick_normals_[v+1]);
						normals_.push_back(brick_normals_[v+2]);						
					}
				}
				for(auto& indice: brick_indices_){
					indices_.push_back(indice + base);
				}
				bricks ++;				
			}
		}
		flipBarrier[(level+1)%2].push_back(half_extents_[0] * 2);
		level ++;
	}
	return ;
}
