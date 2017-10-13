#include "SimpleEngineGameObjectContactCallback.h"
#include <btBulletDynamicsCommon.h>

bool SimpleEngine::GameObjectContactCallback::needsCollision(btBroadphaseProxy* proxy) const
{
    // superclass will check m_collisionFilterGroup and m_collisionFilterMask
    if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
       return false;
    // if passed filters, may also want to avoid contacts between constraints
    return body.checkCollideWithOverride(static_cast<btCollisionObject*>(proxy->m_clientObject));
}

btScalar SimpleEngine::GameObjectContactCallback::addSingleResult(btManifoldPoint& cp,
    const btCollisionObjectWrapper* colObj0,int partId0,int index0,
    const btCollisionObjectWrapper* colObj1,int partId1,int index1)
{
	return 0; // There was a planned purpose for the return value of addSingleResult, but it is not used so you can ignore it.
}