#include <iostream>
using namespace std;

#include "world.h"
#include "cloth.h"

// Soft Body //
Cloth::Cloth(World* world, Shader* shader, float attachWid, float clothLen, uint32_t subd, Head* head, Color color):
	DeadObject(world, shader), width_(attachWid), length_(clothLen), subdivide_(subd) {
	assert(world);
	color_ = color;
	is_soft_ = true;
	if (!shader)
		shader_ = new Shader("shader/cloth.vert", "shader/cloth.frag");
	// Create patch //
	btSoftBody* softBody = btSoftBodyHelpers::CreatePatch(
		world_->bt_soft_info_,
		btVector3(- attachWid, clothLen, - attachWid),
		btVector3(+ attachWid, clothLen, - attachWid),
		btVector3(- attachWid, clothLen, + attachWid),
		btVector3(+ attachWid, clothLen, + attachWid),
		subd, subd, 0, false
	); // 4 + 8
	// Soft Body config
	// softBody->m_cfg.kVC = 0.5;
	// softBody->m_materials[0]->m_kLST = 0.5;
	// softBody->setTotalMass(5);
	// softBody->setPose(true, false);
	world_->bt_world_->addSoftBody(softBody);
	// Attachment //
	float radius = head->getRadius();
	// attached_ = head->bt_object_;
	attached_ = head->delegate_;
	auto trans = attached_->getWorldTransform();
	std::cout << "[Cloth::Cloth(World*, Shader*, float, float, uint32_t, Head*, Color)] " << trans.getOrigin().getX() << ", " << trans.getOrigin().getY() << ", " << trans.getOrigin().getZ() << endl;
	// btCollisionObject* object = head->character_->ghost_object_;
	btVector3 left(-radius, 0, 0.2);
	btVector3 fleft(-0.5 * radius, 0.866 * radius, 0.2);
	btVector3 right(radius, 0, 0.2);
	btVector3 fright(0.5 * radius, 0.866 * radius, 0.2);
	btVector3 back(0, -2, 0.2);

	softBody->appendAnchor(0, static_cast<btRigidBody*>(attached_), fright);
	softBody->appendAnchor((subd - 1) / 2, static_cast<btRigidBody*>(attached_), back);
	softBody->appendAnchor(subd - 1, static_cast<btRigidBody*>(attached_), fleft);
	softBody->appendAnchor(1, static_cast<btRigidBody*>(attached_), left);
	softBody->appendAnchor(subd - 2, static_cast<btRigidBody*>(attached_), right);

	softBody->getCollisionShape()->setUserPointer((void*) softBody);

	bt_object_ = softBody;
}

void Cloth::Draw(Camera* camera, const Light* light){
	auto trans = attached_->getWorldTransform();
	std::cout << "[Cloth::Draw(Camera*)] " << trans.getOrigin().getX() << ", " << trans.getOrigin().getY() << ", " << trans.getOrigin().getZ() << endl;
	InitMesh();
	ImportToGraphics();

	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", color_);
	btTransform transform;
	transform.setIdentity();

	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->GetViewMatrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->GetProjectionMatrix());

	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Cloth::~Cloth() {

}

void Cloth::ImportToGraphics() {
	glBindVertexArray(vao_);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(btScalar), vertices_.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(btScalar), (void *) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), indices_.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}