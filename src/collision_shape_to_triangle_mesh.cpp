#include "collision_shape_to_triangle_mesh.h"

void CollisionShapeToTriangleMesh(
	btCollisionShape *collision_shape, 
	const btTransform &parent_transform, 
	btAlignedObjectArray<btVector3> &vertex_positions, 
	btAlignedObjectArray<btVector3> &vertex_normals, 
	btAlignedObjectArray<int> &indices_out) {

	// todo: support all collision shape types
	switch (collision_shape->getShapeType()) {
		case SOFTBODY_SHAPE_PROXYTYPE: {
			// skip the soft body collision shape for now
			break;
		}
		case STATIC_PLANE_PROXYTYPE: {
			// draw a box, oriented along the plane normal
			const btStaticPlaneShape *static_plane_shape = static_cast<const btStaticPlaneShape *>(collision_shape);
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
				
			int startIndex = vertex_positions.size();
			indices_out.push_back(startIndex+0);
			indices_out.push_back(startIndex+1);
			indices_out.push_back(startIndex+2);
			indices_out.push_back(startIndex+0);
			indices_out.push_back(startIndex+2);
			indices_out.push_back(startIndex+3);

			btVector3 triNormal = parent_transform.getBasis()*planeNormal;
				

			for (int i=0;i<4;i++)
			{
				btVector3 vtxPos;
				btVector3 pos =parent_transform*verts[i];
				vertex_positions.push_back(pos);
				vertex_normals.push_back(triNormal);
			}
			break;
		}
		case TRIANGLE_MESH_SHAPE_PROXYTYPE:
		{
			

			btBvhTriangleMeshShape* trimesh = (btBvhTriangleMeshShape*) collision_shape;
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
							indices_out.push_back(vertex_positions.size());
							vertex_positions.push_back(pos);
							vertex_normals.push_back(triNormal);
						}
					}
					
				}
			}
			
			break;
		}
		default:
		{
			if (collision_shape->isConvex())
			{
				btConvexShape* convex = (btConvexShape*)collision_shape;
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
								indices_out.push_back(vertex_positions.size());
								vertex_positions.push_back(pos);
								vertex_normals.push_back(triNormal);
							}
						}
					}
					delete hull;
				}
			} else
			{
				if (collision_shape->isCompound())
				{
					btCompoundShape* compound = (btCompoundShape*) collision_shape;
					for (int i=0;i<compound->getNumChildShapes();i++)
					{

						btTransform childWorldTrans = parent_transform * compound->getChildTransform(i);
						CollisionShapeToTriangleMesh(compound->getChildShape(i),childWorldTrans,vertex_positions,vertex_normals,indices_out);
					}
				} else
				{
					if (collision_shape->getShapeType()==SDF_SHAPE_PROXYTYPE)
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