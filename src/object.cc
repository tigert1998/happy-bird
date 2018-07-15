#include "opengl_common.h"
#include "bullet_common.h"

#include "object.h"
#include "world.h"
#include "vec_converter.h"
#include <iostream>
using namespace std;
// Base class //
void Object::ImportToPhysics(){
	world_->bt_world_->addRigidBody(dynamic_cast<btRigidBody*>(bt_object_));
}
void Object::DeleteFromPhysics(){
	world_->bt_world_->removeRigidBody(dynamic_cast<btRigidBody*>(bt_object_));
}
void Object::ImportToGraphics(){
	glGenVertexArrays(1, &vao_);

	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(btScalar), vertices_.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(uint32_t), indices_.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(btScalar), (void *) 0);

	glBindVertexArray(0);
}
void Object::Draw(Camera* camera, const btTransform& transform){
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", color_);
	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->GetViewMatrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->GetProjectionMatrix());
	cout << camera->position()[0] << ", "<< camera->position()[1] <<", "<< camera->position()[2] << endl;
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Object::Draw(Camera* camera){
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", color_);
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);

	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->GetViewMatrix());

	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->GetProjectionMatrix());

	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Object::InitMesh(void){
	btTransform parent_transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(parent_transform);
	btCollisionShape* bt_shape = bt_object_->getCollisionShape();
	InitMesh(bt_shape, parent_transform);
	// cout << "Vertices: ";
	// for(auto& i : vertices_){
	// 	cout << i << ", ";
	// }
	// cout << endl;
	// cout << "Indices: ";
	// for(auto& i : indices_){
	// 	cout << i << ", ";
	// }
	// cout << endl;
}

void Object::InitMesh(btCollisionShape* bt_shape, const btTransform& parent_transform){
	// todo: support all collision shape types
	switch (bt_shape->getShapeType()) {
		case SOFTBODY_SHAPE_PROXYTYPE: {
			// skip the soft body collision shape for now
			break;
		}
		case STATIC_PLANE_PROXYTYPE: {
			// draw a box, oriented along the plane normal
			const btStaticPlaneShape *static_plane_shape = static_cast<const btStaticPlaneShape *>(bt_shape);
			btScalar plane_const = static_plane_shape->getPlaneConstant();
			const btVector3& planeNormal = static_plane_shape->getPlaneNormal();
			btVector3 plane_origin = planeNormal * plane_const;
			btVector3 vec0, vec1;
			btPlaneSpace1(planeNormal, vec0, vec1);
			btScalar vecLen = 100.f;
			btVector3 verts[4];

			verts[0] = plane_origin + vec0*vecLen + vec1*vecLen;
			verts[1] = plane_origin - vec0*vecLen + vec1*vecLen;
			verts[2] = plane_origin - vec0*vecLen - vec1*vecLen;
			verts[3] = plane_origin + vec0*vecLen - vec1*vecLen;
			int startIndex = vertices_.size();
			indices_.push_back(startIndex+0);
			indices_.push_back(startIndex+1);
			indices_.push_back(startIndex+2);
			indices_.push_back(startIndex+0);
			indices_.push_back(startIndex+2);
			indices_.push_back(startIndex+3);

			btVector3 triNormal = parent_transform.getBasis()*planeNormal;
				

			for (int i=0;i<4;i++)
			{
				btVector3 vtxPos;
				btVector3 pos =parent_transform*verts[i];
				vertices_.push_back(pos[0]);
				vertices_.push_back(pos[1]);
				vertices_.push_back(pos[2]);
				normals_.push_back(triNormal[0]);
				normals_.push_back(triNormal[1]);
				normals_.push_back(triNormal[2]);
			}
			break;
		}
		case TRIANGLE_MESH_SHAPE_PROXYTYPE:
		{
			

			btBvhTriangleMeshShape* trimesh = (btBvhTriangleMeshShape*) bt_shape;
			btVector3 trimeshScaling = trimesh->getLocalScaling();
			btStridingMeshInterface* meshInterface = trimesh->getMeshInterface();
			btAlignedObjectArray<btVector3> vertices;
			btAlignedObjectArray<int> indices;
				
			for (int partId=0;partId<meshInterface->getNumSubParts();partId++)
			{
					
				const unsigned char *vertexbase = 0;
				int numverts = 0;
				PHY_ScalarType type = PHY_INTEGER;
				int stride = 0;
				const unsigned char *indexbase = 0;
				int indexstride = 0;
				int numfaces = 0;
				PHY_ScalarType indicestype = PHY_INTEGER;
				//PHY_ScalarType indexType=0;
					
				btVector3 triangleVerts[3];
				meshInterface->getLockedReadOnlyVertexIndexBase(&vertexbase,numverts,	type,stride,&indexbase,indexstride,numfaces,indicestype,partId);
				btVector3 aabbMin,aabbMax;
					
				for (int triangleIndex = 0 ; triangleIndex < numfaces;triangleIndex++)
				{
					unsigned int* gfxbase = (unsigned int*)(indexbase+triangleIndex*indexstride);
						
					for (int j=2;j>=0;j--)
					{
							
						int graphicsindex = indicestype==PHY_SHORT?((unsigned short*)gfxbase)[j]:gfxbase[j];
						if (type == PHY_FLOAT)
						{
							float* graphicsbase = (float*)(vertexbase+graphicsindex*stride);
							triangleVerts[j] = btVector3(
															graphicsbase[0]*trimeshScaling.getX(),
															graphicsbase[1]*trimeshScaling.getY(),
															graphicsbase[2]*trimeshScaling.getZ());
						}
						else
						{
							double* graphicsbase = (double*)(vertexbase+graphicsindex*stride);
							triangleVerts[j] = btVector3( btScalar(graphicsbase[0]*trimeshScaling.getX()),
															btScalar(graphicsbase[1]*trimeshScaling.getY()),
															btScalar(graphicsbase[2]*trimeshScaling.getZ()));
						}
					}
					indices.push_back(vertices.size());
					vertices.push_back(triangleVerts[0]);
					indices.push_back(vertices.size());
					vertices.push_back(triangleVerts[1]);
					indices.push_back(vertices.size());
					vertices.push_back(triangleVerts[2]);

					btVector3 triNormal = (triangleVerts[1]-triangleVerts[0]).cross(triangleVerts[2]-triangleVerts[0]);
					btScalar dot = triNormal.dot(triNormal);

					//cull degenerate triangles
					if (dot >= SIMD_EPSILON*SIMD_EPSILON)
					{
						triNormal /= btSqrt(dot);
						for (int v = 0; v < 3; v++)
						{

							btVector3 pos = parent_transform*triangleVerts[v];
							indices_.push_back(vertices_.size() / 3);
							vertices_.push_back(pos[0]);
							vertices_.push_back(pos[1]);
							vertices_.push_back(pos[2]);
							normals_.push_back(triNormal[0]);
							normals_.push_back(triNormal[1]);
							normals_.push_back(triNormal[2]);
						}
					}
					
				}
			}
			
			break;
		}
		default:
		{
			if (bt_shape->isConvex())
			{
				btConvexShape* convex = (btConvexShape*)bt_shape;
				{
					btShapeHull* hull = new btShapeHull(convex);
					hull->buildHull(0.0, 1);

					{
						//int strideInBytes = 9*sizeof(float);
						//int numVertices = hull->numVertices();
						//int numIndices =hull->numIndices();

						for (int t=0;t<hull->numTriangles();t++)
						{

							btVector3 triNormal;

							int index0 = hull->getIndexPointer()[t*3+0];
							int index1 = hull->getIndexPointer()[t*3+1];
							int index2 = hull->getIndexPointer()[t*3+2];
							btVector3 pos0 =parent_transform*hull->getVertexPointer()[index0];
							btVector3 pos1 =parent_transform*hull->getVertexPointer()[index1];
							btVector3 pos2 =parent_transform*hull->getVertexPointer()[index2];
							triNormal = (pos1-pos0).cross(pos2-pos0);
							triNormal.normalize();

							for (int v=0;v<3;v++)
							{
								int index = hull->getIndexPointer()[t*3+v];
								btVector3 pos =parent_transform*hull->getVertexPointer()[index];
								indices_.push_back(vertices_.size() / 3);
								vertices_.push_back(pos[0]);
								vertices_.push_back(pos[1]);
								vertices_.push_back(pos[2]);
								normals_.push_back(triNormal[0]);
								normals_.push_back(triNormal[1]);
								normals_.push_back(triNormal[2]);
							}
						}
					}
					delete hull;
				}
			} else
			{
				if (bt_shape->isCompound())
				{
					btCompoundShape* compound = (btCompoundShape*) bt_shape;
					for (int i=0;i<compound->getNumChildShapes();i++)
					{

						btTransform childWorldTrans = parent_transform * compound->getChildTransform(i);
						InitMesh(compound->getChildShape(i),childWorldTrans);
					}
				} else
				{
					if (bt_shape->getShapeType()==SDF_SHAPE_PROXYTYPE)
					{
						//not yet
					} else
					{
						btAssert(0);
					}
				}
					
			}
		}
	};
}

// Plain Object
Box::Box(World* world, Shader* shader, const btTransform& transform, glm::vec3 half_extents, Color color):
	DeadObject(world,shader),half_extents_(half_extents){
	assert(world_);
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->createRigidBody(
		0,
		transform,
		new btBoxShape( ToBtVec(half_extents) ) );
	// create mesh //
	InitMesh();
	ImportToGraphics();
	// patch shader
	if(!shader){
		shader_ = new Shader("shader/cuboid.vert", "shader/cuboid.frag");
	}
}
#include <cstdio>
void Box::Draw(Camera* camera){
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	printf("Box %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	Object::Draw(camera, transform);
}
Sphere::Sphere(World* world, Shader* shader, const btTransform& transform, float radius, Color color):
	DeadObject(world,shader),radius_(radius){
	assert(world_);
	color_ = color;
	// initialize physics shape //
	bt_object_ = world_->createRigidBody(
		20,
		transform,
		new btSphereShape( btScalar(radius) ));
	// create mesh //
	InitMesh();
	ImportToGraphics();
	if(!shader){
		shader_ = new Shader("shader/sphere.vert", "shader/sphere.frag");
	}
}
void Sphere::Draw(Camera* camera){
	btTransform transform;
	btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	printf("Sphere %f, %f, %f\n", float(transform.getOrigin().getX()), float(transform.getOrigin().getY()), float(transform.getOrigin().getZ()));
	Object::Draw(camera, transform);
}

// Model //
// Model::Model(){
// 	Assimp::Importer importer;
// 	auto scene = importer.ReadFile(path + "/" + file, 
// 		aiProcess_Triangulate | 
// 		aiProcess_FlipUVs | 
// 		aiProcess_CalcTangentSpace
// 	);
// 	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
// 		throw AssimpError(importer.GetErrorString());
// 	}
// 	DFSNode(scene->mRootNode, scene);
// }

// Controller //
// Head::Head(Character* character, float radius, Color color):character_(character), radius_(radiu),color_(color){
// 	// initialize physics shape //
// 	bt_object_ = world_->createRigidBody(
// 		20,
// 		startTransform,
// 		new btSphereShape( btScalar(radius) ));
// 	// create mesh //
// 	InitMesh();
// 	InitControl();
// }


// Soft Body //
// void Cloth::Cloth(float attachWid, float clothLen, uint32_t subd, Head* head){

// 	bt_object_ = btSoftBodyHelpers::CreatePatch(world_->m_softBodyWorldInfo,btVector3(-attachWid,clothLen,-attachWid),
// 		btVector3(+attachWid,clothLen,-attachWid),
// 		btVector3(-attachWid,clothLen,+attachWid),
// 		btVector3(+attachWid,clothLen,+attachWid),subd,subd,0,false); // 4 + 8
// 	world_->getSoftDynamicsWorld()->addSoftBody(bt_object_);

// 	// Attachment
// 	float radius = head->getRadius();
// 	btVector3 left(-radius,0,0.2);
// 	btVector3 fleft(-0.5*radius,0.866*radius,0.2);
// 	btVector3 right(radius,0,0.2);
// 	btVector3 fright(0.5*radius,0.866*radius,0.2);
// 	btVector3 back(0,-2,0.2);
// 	bt_object_->appendAnchor(0,head->bt_object_, fright);
// 	bt_object_->appendAnchor((subd-1)/2,head->bt_object_, back);
// 	bt_object_->appendAnchor(subd-1,head->bt_object_, fleft);
// 	bt_object_->appendAnchor(1,head->bt_object_, left);
// 	bt_object_->appendAnchor(subd-2,head->bt_object_, right);

// 	return ;
// }