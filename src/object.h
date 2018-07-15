#pragma once


// for plain object:
// create through Bullet shape , mesh = shape's mesh, bounding box = caculated box
// for soft body
// create through Bullet shape, mesh update by shape
// for model
// create through assimp, bounding boxes -> Bullet engine, transform update by Bullet


class Object{
	enum{
		kPlainObject,
		kModelObject,
		kSoftObject
	} ObjectType;
	std::vector<Mesh> mesh_;
	std::vector<BoundingBox> box_;
 public:
 	void GetMesh(vertices, indices, normals);

};

class Object{
	Object(){ }
	virtual Object(){ }
	virtual void Bind(btDiscreteDynamicsWorld* world) = 0;
};

class Box: public Object{
	btCollisionShape* btShape;
	btTransform btTransform;
	btDefaultMotionState* btState;
	btRigidBodyConstructionInfo* btInfo;
	btRigidBody* btBody;
	Box(double a, double b, double c):
		btShape(new btCollisionShape(
			btVector3(btScalar(a),btScalar(b),btScalar(c)))),
	{
		btTransform.setIdentity();
		btTransform.setOrigin(btVector3(0,-56,0));
		btScalar mass(0);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			btShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btState = new btDefaultMotionState(btTransform);
		btInfo = new btRigidBody::btRigidBodyConstructionInfo(mass, btState, btShape, localInertia);
		btBody = new btRigidBody(btInfo);
	}
	void Bind(btDiscreteDynamicsWorld* world){
		world->addRigidBody(body);
		return ;
	}
	~Box(){delete btShape; delete btTransform; delete btState; delete btInfo; delete btBody;}
};

class Sphere: public Object{
	btCollisionShape* btShape;
	btTransform btTransform;
	btDefaultMotionState* btState;
	btRigidBodyConstructionInfo* btInfo;
	btRigidBody* btBody;
	Sphere(double a):
		btShape(new btCollisionShape(btScalar(a) )),
	{
		btTransform.setIdentity();
		btTransform.setOrigin(btVector3(0,1,0));
		btScalar mass(1);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			btShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btState = new btDefaultMotionState(btTransform);
		btInfo = new btRigidBody::btRigidBodyConstructionInfo(mass, btState, btShape, localInertia);
		btBody = new btRigidBody(btInfo);
	}
	void Bind(btDiscreteDynamicsWorld* world){
		world->addRigidBody(body);
		return ;
	}
	~Sphere(){delete btShape; delete btTransform; delete btState; delete btInfo; delete btBody;}
};

class Cloth: public Object{
	btCollisionShape* btShape;
	btTransform btTransform;
	btDefaultMotionState* btState;
	btRigidBodyConstructionInfo* btInfo;
	btRigidBody* btBody;
	Cloth(double a, double b, double c):
		btShape(new btCollisionShape(
			btVector3(btScalar(a),btScalar(b),btScalar(c)))),
	{
		btTransform.setIdentity();
		btTransform.setOrigin(btVector3(0,-56,0));
		btScalar mass(0);
		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			btShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btState = new btDefaultMotionState(btTransform);
		btInfo = new btRigidBody::btRigidBodyConstructionInfo(mass, btState, btShape, localInertia);
		btBody = new btRigidBody(btInfo);
	}
	void Bind(btDiscreteDynamicsWorld* world){
		world->addRigidBody(body);
		return ;
	}
	~Cloth(){delete btShape; delete btTransform; delete btState; delete btInfo; delete btBody;}
};

class Cloth: public Object{
	btCollisionObject* base_obj_;
 public:
	Cloth(std::vector<vec3d> control){
		base_obj_ = (btCollisionObject*)btSoftBodyHelpers::CreatePatch()
	}
	void UpdateMesh(void){

	}
};

void			btSoftBodyHelpers::Draw(	btSoftBody* psb,
										btIDebugDraw* idraw,
										int drawflags)
{
	const btScalar		scl=(btScalar)0.1;
	const btScalar		nscl=scl*5;
	const btVector3		lcolor=btVector3(0,0,0);
	const btVector3		ncolor=btVector3(1,1,1);
	const btVector3		ccolor=btVector3(1,0,0);
	int i,j,nj;

		/* Clusters	*/ 
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
#define USE_NEW_CONVEX_HULL_COMPUTER
#ifdef USE_NEW_CONVEX_HULL_COMPUTER
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
						idraw->drawTriangle(computer.vertices[v0],computer.vertices[v1],computer.vertices[v2],color,1);
						edge = edge->getNextEdgeOfFace();
						v0=v1;
						v1=v2;
					};
				}
#else

				HullDesc		hdsc(QF_TRIANGLES,vertices.size(),&vertices[0]);
				HullResult		hres;
				HullLibrary		hlib;
				hdsc.mMaxVertices=vertices.size();
				hlib.CreateConvexHull(hdsc,hres);
				const btVector3	center=average(hres.m_OutputVertices);
				add(hres.m_OutputVertices,-center);
				mul(hres.m_OutputVertices,(btScalar)1);
				add(hres.m_OutputVertices,center);
				for(j=0;j<(int)hres.mNumFaces;++j)
				{
					const int idx[]={hres.m_Indices[j*3+0],hres.m_Indices[j*3+1],hres.m_Indices[j*3+2]};
					idraw->drawTriangle(hres.m_OutputVertices[idx[0]],
						hres.m_OutputVertices[idx[1]],
						hres.m_OutputVertices[idx[2]],
						color,1);
				}
				hlib.ReleaseResult(hres);
#endif

			}
			/* Velocities	*/ 
#if 0
			for(int j=0;j<psb->m_clusters[i].m_nodes.size();++j)
			{
				const btSoftBody::Cluster&	c=psb->m_clusters[i];
				const btVector3				r=c.m_nodes[j]->m_x-c.m_com;
				const btVector3				v=c.m_lv+btCross(c.m_av,r);
				idraw->drawLine(c.m_nodes[j]->m_x,c.m_nodes[j]->m_x+v,btVector3(1,0,0));
			}
#endif
			/* Frame		*/ 
	//		btSoftBody::Cluster& c=*psb->m_clusters[i];
	//		idraw->drawLine(c.m_com,c.m_framexform*btVector3(10,0,0),btVector3(1,0,0));
	//		idraw->drawLine(c.m_com,c.m_framexform*btVector3(0,10,0),btVector3(0,1,0));
	//		idraw->drawLine(c.m_com,c.m_framexform*btVector3(0,0,10),btVector3(0,0,1));
		}
	}
	else
	{
		/* Nodes	*/ 
		if(0!=(drawflags&fDrawFlags::Nodes))
		{
			for(i=0;i<psb->m_nodes.size();++i)
			{
				const btSoftBody::Node&	n=psb->m_nodes[i];
				if(0==(n.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				idraw->drawLine(n.m_x-btVector3(scl,0,0),n.m_x+btVector3(scl,0,0),btVector3(1,0,0));
				idraw->drawLine(n.m_x-btVector3(0,scl,0),n.m_x+btVector3(0,scl,0),btVector3(0,1,0));
				idraw->drawLine(n.m_x-btVector3(0,0,scl),n.m_x+btVector3(0,0,scl),btVector3(0,0,1));
			}
		}
		/* Links	*/ 
		if(0!=(drawflags&fDrawFlags::Links))
		{
			for(i=0;i<psb->m_links.size();++i)
			{
				const btSoftBody::Link&	l=psb->m_links[i];
				if(0==(l.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				idraw->drawLine(l.m_n[0]->m_x,l.m_n[1]->m_x,lcolor);
			}
		}
		/* Normals	*/ 
		if(0!=(drawflags&fDrawFlags::Normals))
		{
			for(i=0;i<psb->m_nodes.size();++i)
			{
				const btSoftBody::Node&	n=psb->m_nodes[i];
				if(0==(n.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
				const btVector3			d=n.m_n*nscl;
				idraw->drawLine(n.m_x,n.m_x+d,ncolor);
				idraw->drawLine(n.m_x,n.m_x-d,ncolor*0.5);
			}
		}
		/* Contacts	*/ 
		if(0!=(drawflags&fDrawFlags::Contacts))
		{
			static const btVector3		axis[]={btVector3(1,0,0),
				btVector3(0,1,0),
				btVector3(0,0,1)};
			for(i=0;i<psb->m_rcontacts.size();++i)
			{		
				const btSoftBody::RContact&	c=psb->m_rcontacts[i];
				const btVector3				o=	c.m_node->m_x-c.m_cti.m_normal*
					(btDot(c.m_node->m_x,c.m_cti.m_normal)+c.m_cti.m_offset);
				const btVector3				x=btCross(c.m_cti.m_normal,axis[c.m_cti.m_normal.minAxis()]).normalized();
				const btVector3				y=btCross(x,c.m_cti.m_normal).normalized();
				idraw->drawLine(o-x*nscl,o+x*nscl,ccolor);
				idraw->drawLine(o-y*nscl,o+y*nscl,ccolor);
				idraw->drawLine(o,o+c.m_cti.m_normal*nscl*3,btVector3(1,1,0));
			}
		}
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
			idraw->drawTriangle((x[0]-c)*scl+c,
				(x[1]-c)*scl+c,
				(x[2]-c)*scl+c,
				col,alp);
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
			idraw->drawTriangle((x[0]-c)*scl+c,(x[1]-c)*scl+c,(x[2]-c)*scl+c,col,alp);
			idraw->drawTriangle((x[0]-c)*scl+c,(x[1]-c)*scl+c,(x[3]-c)*scl+c,col,alp);
			idraw->drawTriangle((x[1]-c)*scl+c,(x[2]-c)*scl+c,(x[3]-c)*scl+c,col,alp);
			idraw->drawTriangle((x[2]-c)*scl+c,(x[0]-c)*scl+c,(x[3]-c)*scl+c,col,alp);
		}	
	}
	}
	/* Anchors	*/ 
	if(0!=(drawflags&fDrawFlags::Anchors))
	{
		for(i=0;i<psb->m_anchors.size();++i)
		{
			const btSoftBody::Anchor&	a=psb->m_anchors[i];
			const btVector3				q=a.m_body->getWorldTransform()*a.m_local;
			drawVertex(idraw,a.m_node->m_x,0.25,btVector3(1,0,0));
			drawVertex(idraw,q,0.25,btVector3(0,1,0));
			idraw->drawLine(a.m_node->m_x,q,btVector3(1,1,1));
		}
		for(i=0;i<psb->m_nodes.size();++i)
		{
			const btSoftBody::Node&	n=psb->m_nodes[i];		
			if(0==(n.m_material->m_flags&btSoftBody::fMaterial::DebugDraw)) continue;
			if(n.m_im<=0)
			{
				drawVertex(idraw,n.m_x,0.25,btVector3(1,0,0));
			}
		}
	}
	

	/* Notes	*/ 
	if(0!=(drawflags&fDrawFlags::Notes))
	{
		for(i=0;i<psb->m_notes.size();++i)
		{
			const btSoftBody::Note&	n=psb->m_notes[i];
			btVector3				p=n.m_offset;
			for(int j=0;j<n.m_rank;++j)
			{
				p+=n.m_nodes[j]->m_x*n.m_coords[j];
			}
			idraw->draw3dText(p,n.m_text);
		}
	}
	/* Node tree	*/ 
	if(0!=(drawflags&fDrawFlags::NodeTree))		DrawNodeTree(psb,idraw);
	/* Face tree	*/ 
	if(0!=(drawflags&fDrawFlags::FaceTree))		DrawFaceTree(psb,idraw);
	/* Cluster tree	*/ 
	if(0!=(drawflags&fDrawFlags::ClusterTree))	DrawClusterTree(psb,idraw);
	/* Joints		*/ 
	if(0!=(drawflags&fDrawFlags::Joints))
	{
		for(i=0;i<psb->m_joints.size();++i)
		{
			const btSoftBody::Joint*	pj=psb->m_joints[i];
			switch(pj->Type())
			{
			case	btSoftBody::Joint::eType::Linear:
				{
					const btSoftBody::LJoint*	pjl=(const btSoftBody::LJoint*)pj;
					const btVector3	a0=pj->m_bodies[0].xform()*pjl->m_refs[0];
					const btVector3	a1=pj->m_bodies[1].xform()*pjl->m_refs[1];
					idraw->drawLine(pj->m_bodies[0].xform().getOrigin(),a0,btVector3(1,1,0));
					idraw->drawLine(pj->m_bodies[1].xform().getOrigin(),a1,btVector3(0,1,1));
					drawVertex(idraw,a0,0.25,btVector3(1,1,0));
					drawVertex(idraw,a1,0.25,btVector3(0,1,1));
				}
				break;
			case	btSoftBody::Joint::eType::Angular:
				{
					//const btSoftBody::AJoint*	pja=(const btSoftBody::AJoint*)pj;
					const btVector3	o0=pj->m_bodies[0].xform().getOrigin();
					const btVector3	o1=pj->m_bodies[1].xform().getOrigin();
					const btVector3	a0=pj->m_bodies[0].xform().getBasis()*pj->m_refs[0];
					const btVector3	a1=pj->m_bodies[1].xform().getBasis()*pj->m_refs[1];
					idraw->drawLine(o0,o0+a0*10,btVector3(1,1,0));
					idraw->drawLine(o0,o0+a1*10,btVector3(1,1,0));
					idraw->drawLine(o1,o1+a0*10,btVector3(0,1,1));
					idraw->drawLine(o1,o1+a1*10,btVector3(0,1,1));
					break;
				}
				default:
				{
				}
					
			}		
		}
	}
}

class Sphere: public Object{

};

class Box: public Object{

};