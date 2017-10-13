#include <btBulletDynamicsCommon.h>

struct GameObjectContactCallback : public btCollisionWorld::ContactResultCallback
{
private:
	GameObject* go1;
	GameObject* go2;
public:
	GameObjectContactCallback(GameObject* gameObject1, GameObject2);

	virtual bool needsCollision(btBroadphaseProxy* proxy) const;

    virtual btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0,int partId0,int index0,
        const btCollisionObjectWrapper* colObj1,int partId1,int index1);
}