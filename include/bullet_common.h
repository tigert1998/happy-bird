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

#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/BulletCollision/CollisionShapes/btShapeHull.h"
