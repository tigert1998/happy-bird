#include "world.h"
#include "cloth.h"

// Soft Body //
Cloth::Cloth(World* world, Shader* shader, float attachWid, float clothLen, uint32_t subd, Head* head, Color color):
	DeadObject(world, shader), 
	width_(attachWid),length_(clothLen),subdivide_(subd){
	assert(world);
	color_ = color;
	is_soft_ = true;
	if(!shader)shader_ = new Shader("shader/common.vert", "shader/common.frag");
	// Create patch //
	btSoftBody* softBody = btSoftBodyHelpers::CreatePatch(world_->bt_soft_info_,btVector3(-attachWid,clothLen,-attachWid),
		btVector3(+attachWid,clothLen,-attachWid),
		btVector3(-attachWid,clothLen,+attachWid),
		btVector3(+attachWid,clothLen,+attachWid),subd,subd,0,false); // 4 + 8
	// Soft Body config
	// softBody->m_cfg.kVC = 0.5;
	// softBody->m_materials[0]->m_kLST = 0.5;
	// softBody->setTotalMass(5);
	// softBody->setPose(true, false);
	world_->bt_world_->addSoftBody(softBody);
	// Attachment //
	float radius = head->getRadius();
	btVector3 left(-radius,0,0.2);
	btVector3 fleft(-0.5*radius,0.866*radius,0.2);
	btVector3 right(radius,0,0.2);
	btVector3 fright(0.5*radius,0.866*radius,0.2);
	btVector3 back(0,-2,0.2);
	softBody->appendAnchor(0, static_cast<btRigidBody*>(head->bt_object_), fright);
	softBody->appendAnchor((subd-1)/2, static_cast<btRigidBody*>(head->bt_object_), back);
	softBody->appendAnchor(subd-1, static_cast<btRigidBody*>(head->bt_object_), fleft);
	softBody->appendAnchor(1, static_cast<btRigidBody*>(head->bt_object_), left);
	softBody->appendAnchor(subd- 2, static_cast<btRigidBody*>(head->bt_object_), right);

	softBody->getCollisionShape()->setUserPointer((void*)softBody);

	bt_object_ = softBody;

	// Init at draw
	return ;
}
void Cloth::Draw(Camera* camera){
	InitMesh();
	ImportToGraphics();
	Object::Draw(camera);
}