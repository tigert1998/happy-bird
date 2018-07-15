#pragma once

#include "bullet_common.h"

class btCollisionShape;

void CollisionShapeToTriangleMesh(
	btCollisionShape *collision_shape, 
	const btTransform& parent_transform, 
	btAlignedObjectArray<btVector3> &vertex_positions, 
	btAlignedObjectArray<btVector3> &vertex_normals, 
	btAlignedObjectArray<int> &indices_out
);
