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
	Object::InitMesh();
	tex_coords_.clear();
	for (int i = 0; i < vertices_.size(); i += 3) {
		tex_coords_.push_back(vertices_[i + 0] < 0 ? 0 : 1);
		tex_coords_.push_back(vertices_[i + 1] < 0 ? 0 : 1);
	}
}

void Hero::ImportToGraphics() {
	Object::ImportToGraphics();

	glBindVertexArray(vao_);
	glGenBuffers(1, &tex_coord_vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo_);
	glBufferData(GL_ARRAY_BUFFER, tex_coords_.size() * sizeof(btScalar), tex_coords_.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(btScalar), (void *) 0);
	glBindVertexArray(0);
}

Hero::Hero(World* world, Shader* shader, Material* material, const btTransform& transform, float w, float h):
	LivingObject(world, shader, material), width_(w), height_(h) {
	assert(world_);
	is_soft_ = false;
	// initialize physics shape
	bt_object_ = world_->CreateRigidBody(
		200,
		transform,
		new btBoxShape(btVector3(width_ / 2, height_ / 2, thickness_ / 2))
	);
	if (!shader) {
		shader_ = new Shader("shader/hero.vert", "shader/hero.frag");
	}
	// create mesh
	InitMesh();
	ImportToGraphics();
	// Bind to new character
	character_ = new CharacterImpl(world_, transform, bt_object_);
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
	return character_->GetDelegate()->getWorldTransform().getOrigin();
}

void Hero::Draw(Camera* camera, const LightCollection* light_collection) {
	btCollisionObject* obj = character_->GetDelegate();
	btTransform trans = obj->getWorldTransform();
	Object::Draw(camera, trans, light_collection);
}
