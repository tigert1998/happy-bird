#include "opengl_common.h"
#include "bullet_common.h"

#include "object.h"
#include "world.h"
#include "vector_utility.h"
#include <iostream>
#include <cstdio>
using namespace std;
// Base class //
Object::Object(World* w, Shader* shader):
	world_(w),
	shader_(shader),
	bt_object_(nullptr){
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glGenBuffers(1, &ebo_);
	}
Object::~Object(){
	// delete world_;
	delete shader_;
	delete bt_object_;
}
void Object::ImportToPhysics(){
	world_->bt_world_->addRigidBody(dynamic_cast<btRigidBody*>(bt_object_));
}
void Object::DeleteFromPhysics(){
	world_->bt_world_->removeRigidBody(dynamic_cast<btRigidBody*>(bt_object_));
}
void Object::ImportToGraphics(){

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
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Object::Draw(Camera* camera){
	shader_->Use();
	shader_->SetUniform<glm::vec3>("uColor", color_);
	btTransform transform;
	if(!is_soft_){
		btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(transform);
	}
	else{
		transform.setIdentity();
	}
	shader_->SetUniform<btTransform>("uModelMatrix", transform);
	shader_->SetUniform<glm::mat4>("uViewMatrix", camera->GetViewMatrix());

	shader_->SetUniform<glm::mat4>("uProjectionMatrix", camera->GetProjectionMatrix());

	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Object::InitMesh(void){
	vertices_.clear();
	indices_.clear();
	if(!is_soft_){	
		btTransform parent_transform;
		btRigidBody::upcast(bt_object_)->getMotionState()->getWorldTransform(parent_transform);
		btCollisionShape* bt_shape = bt_object_->getCollisionShape();
		InitRigidMesh(bt_shape, parent_transform);
	}
	else{
		InitSoftMesh( dynamic_cast<btSoftBody*>(bt_object_) );
	}
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
						InitRigidMesh(compound->getChildShape(i),childWorldTrans);
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
void Object::InitSoftMesh(btSoftBody* psb){
	int drawflags = 0x0004;
	const btScalar		scl=(btScalar)0.1;
	const btScalar		nscl=scl*5;
	const btVector3		lcolor=btVector3(0,0,0);
	const btVector3		ncolor=btVector3(1,1,1);
	const btVector3		ccolor=btVector3(1,0,0);
	int i,j,nj;
	if(0!=(drawflags&fDrawFlags::Clusters))
	{
		srand(1806);
		for(i=0;i<psb->m_clusters.size();++i)
		{
			if(psb->m_clusters[i]->m_collide)
			{
				btVector3						color(	rand()/(btScalar)RAND_MAX,
					rand()/(btScalar)RAND_MAX,
					rand()/(btScalar)RAND_MAX);
				color=color.normalized()*0.75;
				btAlignedObjectArray<btVector3>	vertices;
				vertices.resize(psb->m_clusters[i]->m_nodes.size());
				for(j=0,nj=vertices.size();j<nj;++j)
				{				
					vertices[j]=psb->m_clusters[i]->m_nodes[j]->m_x;
				}
				btConvexHullComputer	computer;
				int stride = sizeof(btVector3);
				int count = vertices.size();
				btScalar shrink=0.f;
				btScalar shrinkClamp=0.f;
				computer.compute(&vertices[0].getX(),stride,count,shrink,shrinkClamp);
				for (int i=0;i<computer.faces.size();i++)
				{

					int face = computer.faces[i];
					//printf("face=%d\n",face);
					const btConvexHullComputer::Edge*  firstEdge = &computer.edges[face];
					const btConvexHullComputer::Edge*  edge = firstEdge->getNextEdgeOfFace();

					int v0 = firstEdge->getSourceVertex();
					int v1 = firstEdge->getTargetVertex();
					while (edge!=firstEdge)
					{
						int v2 = edge->getTargetVertex();
						addTriangle(computer.vertices[v0],computer.vertices[v1],computer.vertices[v2]);
						edge = edge->getNextEdgeOfFace();
						v0=v1;
						v1=v2;
					};
				}
			}
		}
	}
	else
	{
		/* Faces	*/ 
		if(0!=(drawflags&fDrawFlags::Faces))
		{
			const btScalar	scl=(btScalar)0.8;
			const btScalar	alp=(btScalar)1;
			const btVector3	col(0,(btScalar)0.7,0);
			for(i=0;i<psb->m_faces.size();++i)
			{
				const btSoftBody::Face&	f=psb->m_faces[i];
				if(0==(f.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				const btVector3			x[]={f.m_n[0]->m_x,f.m_n[1]->m_x,f.m_n[2]->m_x};
				const btVector3			c=(x[0]+x[1]+x[2])/3;
				addTriangle((x[0]-c)*scl+c,
					(x[1]-c)*scl+c,
					(x[2]-c)*scl+c);
			}	
		}
		/* Tetras	*/ 
		if(0!=(drawflags&fDrawFlags::Tetras))
		{
			const btScalar	scl=(btScalar)0.8;
			const btScalar	alp=(btScalar)1;
			const btVector3	col((btScalar)0.3,(btScalar)0.3,(btScalar)0.7);
			for(int i=0;i<psb->m_tetras.size();++i)
			{
				const btSoftBody::Tetra&	t=psb->m_tetras[i];
				if(0==(t.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				const btVector3				x[]={t.m_n[0]->m_x,t.m_n[1]->m_x,t.m_n[2]->m_x,t.m_n[3]->m_x};
				const btVector3				c=(x[0]+x[1]+x[2]+x[3])/4;
				addTriangle((x[0]-c)*scl+c,(x[1]-c)*scl+c,(x[2]-c)*scl+c);
				addTriangle((x[0]-c)*scl+c,(x[1]-c)*scl+c,(x[3]-c)*scl+c);
				addTriangle((x[1]-c)*scl+c,(x[2]-c)*scl+c,(x[3]-c)*scl+c);
				addTriangle((x[2]-c)*scl+c,(x[0]-c)*scl+c,(x[3]-c)*scl+c);
			}	
		}
	} // end of else if clause
}

