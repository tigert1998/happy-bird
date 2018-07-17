#pragma once

#if defined(_MSC_VER) || defined(__WIN32) || defined(__WIN64)
#pragma comment(lib,"BulletDynamics.lib")
#pragma comment(lib,"BulletExampleBrowserLib.lib")
#pragma comment(lib,"App_BulletExampleBrowser.lib")
#pragma comment(lib,"BulletInverseDynamics.lib")
#pragma comment(lib,"Bullet3Collision.lib")
#pragma comment(lib,"BulletInverseDynamicsUtils.lib")
#pragma comment(lib,"Bullet3Common.lib")
#pragma comment(lib,"BulletSoftBody.lib")
#pragma comment(lib,"Bullet3Dynamics.lib")
#pragma comment(lib,"BussIK.lib")
#pragma comment(lib,"Bullet3Geometry.lib")
#pragma comment(lib,"gwen.lib")
#pragma comment(lib,"Bullet3OpenCL_clew.lib")
#pragma comment(lib,"LinearMath.lib")
#pragma comment(lib,"BulletCollision.lib")
#pragma comment(lib,"OpenGL_Window.lib ")
#endif

// Bullet essential
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

// Soft Body and helpers
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "LinearMath/btQuickprof.h"
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodySolvers.h"
#include "BulletSoftBody/btDefaultSoftBodySolver.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "LinearMath/btConvexHullComputer.h"

// Character control
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

