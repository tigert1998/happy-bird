#include <iostream>
using std::cout;
using std::endl;

#include "opengl_common.h"
#include "bullet_common.h"

#include "object.h"
#include "world.h"
#include "camera.h"
#include "debug_utility/log.h"

// Base class //
Object::Object(World* w, Shader* shader, Material* material, uint32_t stride, bool soft):
		world_(w),
		shader_(shader),
		material_(material),
		bt_object_(nullptr),
		stride_(stride),
		is_soft_(soft) {
	// Init in Graphics
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
}

Object::~Object(){
	// cout << __func__ << endl;
	// delete from Graphics
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	// delete from bullet
	if(bt_object_ && world_ && world_->bt_world_ && !World::exit)world_->bt_world_->removeCollisionObject(bt_object_);
	// DeleteFromPhysics();
	delete shader_;
	delete bt_object_;
}
void Object::InitRigidMesh(void){
	assert(stride_ >= 6);
	btTransform transform;
	transform.setIdentity();
	InitRigidMesh(bt_object_->getCollisionShape(), transform);
}
void Object::AddTriangle(const btVector3& a, const btVector3& b, const btVector3& c) {
	indices_.push_back(data_.size() / stride_);
	indices_.push_back(data_.size() / stride_ + 1);
	indices_.push_back(data_.size() / stride_ + 2);
	data_.push_back(a[0]);
	data_.push_back(a[1]);
	data_.push_back(a[2]);
	for(int i = 3; i <= stride_; i++)data_.push_back(0);
	data_.push_back(b[0]);
	data_.push_back(b[1]);
	data_.push_back(b[2]);
	for(int i = 3; i <= stride_; i++)data_.push_back(0);
	data_.push_back(c[0]);
	data_.push_back(c[1]);
	data_.push_back(c[2]);
	for(int i = 3; i <= stride_; i++)data_.push_back(0);
}
void Object::InitRigidMesh(btCollisionShape* bt_shape, const btTransform& parent_transform){
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

			verts[0] = plane_origin + vec0 * vecLen + vec1 * vecLen;
			verts[1] = plane_origin - vec0 * vecLen + vec1 * vecLen;
			verts[2] = plane_origin - vec0 * vecLen - vec1 * vecLen;
			verts[3] = plane_origin + vec0 * vecLen - vec1 * vecLen;
			int startIndex = data_.size() / stride_;
			indices_.push_back(startIndex + 0);
			indices_.push_back(startIndex + 1);
			indices_.push_back(startIndex + 2);
			indices_.push_back(startIndex + 0);
			indices_.push_back(startIndex + 2);
			indices_.push_back(startIndex + 3);

			btVector3 triNormal = parent_transform.getBasis()*planeNormal;
				

			for (int i = 0; i < 4; i++) {
				btVector3 vtxPos;
				btVector3 pos =parent_transform * verts[i];
				data_.push_back(pos[0]);
				data_.push_back(pos[1]);
				data_.push_back(pos[2]);
				data_.push_back(triNormal[0]);
				data_.push_back(triNormal[1]);
				data_.push_back(triNormal[2]);
				for(int j = 6; j < stride_; j++)data_.push_back(0);
			}
			break;
		}
		case TRIANGLE_MESH_SHAPE_PROXYTYPE: {
			btBvhTriangleMeshShape* trimesh = (btBvhTriangleMeshShape*) bt_shape;
			btVector3 trimeshScaling = trimesh->getLocalScaling();
			btStridingMeshInterface* meshInterface = trimesh->getMeshInterface();
			btAlignedObjectArray<btVector3> vertices;
			btAlignedObjectArray<int> indices;
				
			for (int partId = 0; partId<meshInterface->getNumSubParts(); partId++) {
				const unsigned char *vertexbase = 0;
				int numverts = 0;
				PHY_ScalarType type = PHY_INTEGER;
				int stride = 0;
				const unsigned char* indexbase = 0;
				int indexstride = 0;
				int numfaces = 0;
				PHY_ScalarType indicestype = PHY_INTEGER;
					
				btVector3 triangleVerts[3];
				meshInterface->getLockedReadOnlyVertexIndexBase(
					&vertexbase,
					numverts,
					type,
					stride,
					&indexbase,
					indexstride,
					numfaces,
					indicestype,
					partId
				);
				btVector3 aabbMin,aabbMax;
					
				for (int triangleIndex = 0; triangleIndex < numfaces; triangleIndex++) {
					unsigned int* gfxbase = (unsigned int*) (indexbase + triangleIndex * indexstride);
						
					for (int j = 2; j >= 0; j--) {
							
						int graphicsindex = indicestype == PHY_SHORT ? ((unsigned short*) gfxbase)[j] : gfxbase[j];
						if (type == PHY_FLOAT) {
							float* graphicsbase = (float*) (vertexbase+graphicsindex * stride);
							triangleVerts[j] = btVector3(
								graphicsbase[0] * trimeshScaling.getX(),
								graphicsbase[1] * trimeshScaling.getY(),
								graphicsbase[2] * trimeshScaling.getZ()
							);
						} else {
							double* graphicsbase = (double*) (vertexbase+graphicsindex * stride);
							triangleVerts[j] = btVector3( 
								btScalar(graphicsbase[0] * trimeshScaling.getX()),
								btScalar(graphicsbase[1] * trimeshScaling.getY()),
								btScalar(graphicsbase[2] * trimeshScaling.getZ())
							);
						}
					}
					indices.push_back(vertices.size());
					vertices.push_back(triangleVerts[0]);
					indices.push_back(vertices.size());
					vertices.push_back(triangleVerts[1]);
					indices.push_back(vertices.size());
					vertices.push_back(triangleVerts[2]);

					btVector3 triNormal = (triangleVerts[1] - triangleVerts[0]).cross(triangleVerts[2] - triangleVerts[0]);
					btScalar dot = triNormal.dot(triNormal);

					//cull degenerate triangles
					if (dot >= SIMD_EPSILON*SIMD_EPSILON) {
						triNormal /= btSqrt(dot);
						for (int v = 0; v < 3; v++) {
							btVector3 pos = parent_transform*triangleVerts[v];
							indices_.push_back(data_.size() / stride_);
							data_.push_back(pos[0]);
							data_.push_back(pos[1]);
							data_.push_back(pos[2]);
							data_.push_back(triNormal[0]);
							data_.push_back(triNormal[1]);
							data_.push_back(triNormal[2]);
							for(int j = 6; j < stride_; j++)data_.push_back(0);
						}
					}
					
				}
			}
			break;
		}
		default: {
			if (bt_shape->isConvex()) {
				btConvexShape* convex = (btConvexShape*)bt_shape;
				{
					btShapeHull* hull = new btShapeHull(convex);
					hull->buildHull(0.0, 1);

					{
						//int strideInBytes = 9*sizeof(float);
						//int numVertices = hull->numVertices();
						//int numIndices =hull->numIndices();

						for (int t = 0; t < hull->numTriangles(); t++) {
							btVector3 triNormal;

							int index0 = hull->getIndexPointer()[t * 3 + 0];
							int index1 = hull->getIndexPointer()[t * 3 + 1];
							int index2 = hull->getIndexPointer()[t * 3 + 2];
							btVector3 pos0 = parent_transform * hull->getVertexPointer()[index0];
							btVector3 pos1 = parent_transform * hull->getVertexPointer()[index1];
							btVector3 pos2 = parent_transform * hull->getVertexPointer()[index2];
							triNormal = (pos1 - pos0).cross(pos2 - pos0);
							triNormal.normalize();

							for (int v = 0; v < 3; v++) {
								int index = hull->getIndexPointer()[t * 3 + v];
								btVector3 pos = parent_transform * hull->getVertexPointer()[index];
								indices_.push_back(data_.size() / stride_);
								data_.push_back(pos[0]);
								data_.push_back(pos[1]);
								data_.push_back(pos[2]);
								data_.push_back(triNormal[0]);
								data_.push_back(triNormal[1]);
								data_.push_back(triNormal[2]);
								for(int j = 6; j < stride_; j++)data_.push_back(0);
							}
						}
					}
					delete hull;
				}
			} else {
				if (bt_shape->isCompound()) {
					btCompoundShape* compound = (btCompoundShape*) bt_shape;
					for (int i = 0; i < compound->getNumChildShapes(); i++) {
						btTransform childWorldTrans = parent_transform * compound->getChildTransform(i);
						InitRigidMesh(compound->getChildShape(i),childWorldTrans);
					}
				} else {
					if (bt_shape->getShapeType() == SDF_SHAPE_PROXYTYPE) {
						//not yet
					} else {
						btAssert(0);
					}
				}
			}
		}
	};
}

void Object::InitSoftMesh(void) {
	assert(stride_ >= 3);
	btSoftBody* psb = dynamic_cast<btSoftBody*>(bt_object_);
	int drawflags = 0x0004;
	const btScalar scl = (btScalar) 0.1;
	const btScalar nscl = scl * 5;
	const btVector3 lcolor = btVector3(0, 0, 0);
	const btVector3	ncolor = btVector3(1, 1, 1);
	const btVector3	ccolor = btVector3(1, 0, 0);
	int i, j, nj;
	if (0 != (drawflags & fDrawFlags::Clusters)) {
		srand(1806);
		for (i = 0; i < psb->m_clusters.size(); ++i) {
			if (psb->m_clusters[i]->m_collide) {
				btVector3 color(
					rand() / (btScalar) RAND_MAX,
					rand() / (btScalar) RAND_MAX,
					rand() / (btScalar) RAND_MAX
				);
				color = color.normalized() * 0.75;
				btAlignedObjectArray<btVector3>	vertices;
				vertices.resize(psb->m_clusters[i]->m_nodes.size());
				for(j = 0, nj = vertices.size(); j < nj; ++j) {
					vertices[j] = psb->m_clusters[i]->m_nodes[j]->m_x;
				}
				btConvexHullComputer computer;
				int stride = sizeof(btVector3);
				int count = vertices.size();
				btScalar shrink = 0.0f;
				btScalar shrinkClamp = 0.0f;
				computer.compute(&vertices[0].getX(), stride,count, shrink, shrinkClamp);
				for (int i = 0; i < computer.faces.size(); i++) {
					int face = computer.faces[i];
					//printf("face=%d\n",face);
					const btConvexHullComputer::Edge* firstEdge = &computer.edges[face];
					const btConvexHullComputer::Edge* edge = firstEdge->getNextEdgeOfFace();

					int v0 = firstEdge->getSourceVertex();
					int v1 = firstEdge->getTargetVertex();
					while (edge != firstEdge) {
						int v2 = edge->getTargetVertex();
						AddTriangle(computer.vertices[v0], computer.vertices[v1], computer.vertices[v2]);
						edge = edge->getNextEdgeOfFace();
						v0 = v1;
						v1 = v2;
					}
				}
			}
		}
	} else {
		/* Faces */ 
		if (0 != (drawflags & fDrawFlags::Faces)) {
			const btScalar scl = (btScalar) 0.8;
			const btScalar alp = (btScalar) 1;
			const btVector3	col(0, (btScalar) 0.7, 0);
			for (i = 0; i < psb->m_faces.size(); ++i) {
				const btSoftBody::Face&	f=psb->m_faces[i];
				if (0 == (f.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				const btVector3 x[] = {
					f.m_n[0]->m_x,
					f.m_n[1]->m_x,
					f.m_n[2]->m_x
				};
				const btVector3 c = (x[0] + x[1] + x[2]) / 3;
				AddTriangle(
					(x[0] - c) * scl + c,
					(x[1] - c) * scl + c,
					(x[2] - c) * scl + c
				);
			}
		}
		/* Tetras */ 
		if (0 != (drawflags & fDrawFlags::Tetras)) {
			const btScalar scl = (btScalar) 0.8;
			const btScalar alp = (btScalar) 1;
			const btVector3	col((btScalar) 0.3, (btScalar) 0.3, (btScalar) 0.7);
			for (int i = 0; i < psb->m_tetras.size(); ++i) {
				const btSoftBody::Tetra& t = psb->m_tetras[i];
				if (0 == (t.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				const btVector3	x[] = {
					t.m_n[0]->m_x,
					t.m_n[1]->m_x,
					t.m_n[2]->m_x,
					t.m_n[3]->m_x
				};
				const btVector3 c = (x[0] + x[1] + x[2] + x[3]) / 4;
				AddTriangle((x[0] - c) * scl + c, (x[1] - c) * scl + c, (x[2] - c) * scl + c);
				AddTriangle((x[0] - c) * scl + c, (x[1] - c) * scl + c, (x[3] - c) * scl + c);
				AddTriangle((x[1] - c) * scl + c, (x[2] - c) * scl + c, (x[3] - c) * scl + c);
				AddTriangle((x[2] - c) * scl + c, (x[0] - c) * scl + c, (x[3] - c) * scl + c);
			}	
		}
	}
}

btVector3 Object::GetOrigin(void) {
	return GetTransform().getOrigin();
}

btTransform Object::GetTransform(void) {
	btTransform transform;
	if (!is_soft_ && bt_object_) {
		btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
		return transform;
	}
	else if (anchor_.lock()) {
		transform.setIdentity();
		transform.setOrigin(offset_);
		return transform * (anchor_.lock())->GetTransform();
	}
	else {
		transform.setIdentity();
		return transform;
	}
}

void Object::Draw(Camera* camera, const LightCollection* light_collection){
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uEye.position", camera->position());
	shader_->SetUniform<LightCollection>("uLightCollection", *light_collection);
	shader_->SetUniform<Material>("uMaterial", *material_);
	shader_->SetUniform<btTransform>("uModelMatrix", GetTransform());
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->view_matrix());
	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->projection_matrix());
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
btCollisionObject* Object::GetBulletObject(void){
	return bt_object_;
}
void Object::Attach(std::weak_ptr<Object> target, const btVector3& offset){
	assert(!is_soft_); // soft body live by Bullet
	anchor_ = target;
	offset_ = offset;
	if(bt_object_){
		world_->bt_world_->removeCollisionObject(bt_object_);
		bt_object_ = nullptr;
	}
}
void Object::ActivateControl(void){
	bt_object_->setActivationState(ACTIVE_TAG);
}

btVector3 Object::GetVelocity(void){
	if(!bt_object_ || is_soft_)return btVector3(0, 0, 0);
	else{
		return dynamic_cast<btRigidBody*>(bt_object_)->getLinearVelocity();
	}
}
void Object::SetVelocity(const btVector3& v){
	if(!bt_object_)return ;
	if(is_soft_){
		dynamic_cast<btSoftBody*>(bt_object_)->setVelocity(v);
	}
	else{
		dynamic_cast<btRigidBody*>(bt_object_)->setLinearVelocity(v);
	}
}