#include "hero.h"
#include "world.h"
#include "camera.h"
#include "debug_utility/log.h"

float Hero::width() const {
	return width_;
}

float Hero::height() const {
	return height_;
}

float Hero::thickness_ = 0.5;

Hero::~Hero() { }

void Hero::InitMesh() {
	Object::InitRigidMesh(); // vertice + normal
	// texture
	for (int i = 0; i + stride_ - 1 < data_.size(); i += stride_) {
		data_[i + 6] = data_[i + 0] < 0 ? 0 : 1;
		data_[i + 7] = data_[i + 1] < 0 ? 0 : 1;
	}
}

void Hero::ImportToGraphics() {
	glBindVertexArray(vao_);
	// Bind indice
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), indices_.data(), GL_STATIC_DRAW);
	// Bind data
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, data_.size() * sizeof(float), data_.data(), GL_STATIC_DRAW);	
	// Basic Attrib: vertex coords
	glVertexAttribPointer(0, 3, GL_FLOAT, false, stride_ * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// Normal attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, false, stride_ * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, false, stride_ * sizeof(float), (void *) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Unbind vao
	glBindVertexArray(0);
}

Hero::Hero(World* world, Shader* shader, Material* material, const btTransform& transform, float w, float h):
	Object(world, shader, material, 8, false), width_(w), height_(h) {
	std::cout << "InitHero" << std::endl;
	assert(world_);
	// initialize physics shape
	bt_object_ = world_->CreateRigidBody(
		200,
		transform,
		new btBoxShape(btVector3(width_ / 2, height_ / 2, thickness_ / 2))
	);
	if (!shader) {
		shader_ = new Shader("shader\\hero.vert", "shader\\hero.frag");
	}
	// create mesh
	InitMesh(); // rigid mesh + texture
	ImportToGraphics();
	// Add constraint
	btVector3 pivotInA(0, World::character_height, 0);
	btTransform tmpTrans(btTransform::getIdentity());
	btGeneric6DofConstraint* level_constraint = new btGeneric6DofConstraint(*(dynamic_cast<btRigidBody*>(bt_object_)), tmpTrans, true);
	btVector3 translation_lower(1, 1, 1);
	btVector3 translation_upper(-1, -1, -1);
	btVector3 rotation_lower(0, 0, 0);
	btVector3 rotation_upper(0, 0, 0);
	level_constraint->setLinearLowerLimit(translation_lower);
	level_constraint->setLinearUpperLimit(translation_upper);
	level_constraint->setAngularLowerLimit(rotation_lower);
	level_constraint->setAngularUpperLimit(rotation_upper);
	world_->bt_world()->addConstraint((btTypedConstraint*) level_constraint);
}

btVector3 Hero::GetOrigin() {
	return bt_object_->getWorldTransform().getOrigin();
}
btTransform Hero::GetTransform(void){
	return bt_object_->getWorldTransform();
}

