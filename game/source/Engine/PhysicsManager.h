#ifndef _PHYSICS_MANAGER_H
#define _PHYSICS_MANAGER_H

#include "btBulletDynamicsCommon.h"
#include "SimBase.h"
#include "GameObject.h"
#include "SceneObject.h"
#include <map>
#include <set>
#include <vector>
#include <string>

//typedef unsigned int ColliderID;
//typedef unsigned int RigidBodyID;

class Game;
class PhysicsManager;

class PhysicsProxy : public btMotionState
{
public:
	SceneObject&						mSceneObject;
	ColliderID							mCollisionShapeID;
	RigidBodyID							mRigidBodyID;
	float								mMass;
	bool								mIsRigidBody;
	bool								mIsTrigger;
	std::map<RigidBodyID,unsigned int>	mTriggerOccupants;
	
public:
	PhysicsProxy(SceneObject* so, ColliderID cID, bool isRigidBody = true, bool isTrigger = false ,float pMass = 0.0f);
	
	virtual ~PhysicsProxy()
	{
		
	}
	
	void  OnCollision(PhysicsProxy& other, const btVector3& contactPointWorldCoord, const btVector3& normal);
	void  UpdateTriggerOccupants();
	
	virtual void	getWorldTransform(btTransform& worldTrans ) const
	{
		IvMatrix44 m;
		mSceneObject.GetTransformForPhysics(m);
		worldTrans.setFromOpenGLMatrix(m);
	}
	
	//Bullet only calls the update of worldtransform for active objects
	virtual void	setWorldTransform(const btTransform& worldTrans)
	{
		IvMatrix44 m;
		worldTrans.getOpenGLMatrix(m);
		mSceneObject.SetTransformFromPhysics(m);
	}
	
	//For set<> compare/sort operations
	bool operator()(const PhysicsProxy* p1, const PhysicsProxy* p2) const
	{
		return p1->mRigidBodyID < p2->mRigidBodyID;
	}
};


class PhysicsManager : public SimBase
{
protected:
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher*					mDispatcher;
	btBroadphaseInterface*					mOverlappingPairCache;
	btSequentialImpulseConstraintSolver*	mSolver;
	btDynamicsWorld*						mDynamicsWorld;
	//btCollisionWorld*						mCollisionWorld;
	
	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	std::vector<btCollisionShape*>			mCollisionShapes;
	//std::vector<btCollisionObject*>		mCollisionObjects;//Part of collision world
	std::vector<btRigidBody*>				mRigidBodies;
	std::set<PhysicsProxy*>	mActiveTriggers;
	std::vector<PhysicsProxy*> mPendingTriggerDeletions;
	
	Game&										mGame;
public:
	PhysicsManager(Game& game);
	~PhysicsManager();
	
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	void Start(unsigned int time);
	void Update();
	
	ColliderID ConstructCollider(PersistentData& storageData);
	ColliderID ConstructBoxCollider(const IvVector3& extents);
	ColliderID ConstructSphereCollider(float radius);
	//void ConstructCollisionObject(PhysicsProxy& pProxy);
	RigidBodyID ConstructRigidBody(PhysicsProxy* pProxy, ColliderID pColliderID, float pMass);
	void ConstructAndAssignRigidBody(PhysicsProxy& pProxy);
	
	void SetGravity(float x, float y, float z);
	void ProcessCollisions();
	void ProcessTriggers();
	void TrackTrigger(PhysicsProxy* proxy);
	void StopTrackingTrigger(PhysicsProxy* proxy);
	
	 btCollisionShape* GetCollisionShape(unsigned int shapeID)
	{
		btCollisionShape* shape = NULL;
		if(0 <= shapeID && shapeID < mCollisionShapes.size())
			shape = mCollisionShapes[shapeID];
		
		return shape;
	};
	
	btRigidBody* GetRigidBody(RigidBodyID rbID)
	{
		btRigidBody* rb = NULL;
		if(0 <= rbID && rbID < mRigidBodies.size())
			rb = mRigidBodies[rbID];
		return rb;
	};
	
	/*btCollisionObject* GetCollisionObject(ColliderID coID)
	{
		btCollisionObject* co = NULL;
		if(0 <= coID && coID < mCollisionObjects.size())
			co = mCollisionObjects[coID];
		return co;
	};*/
	
private:
	PhysicsManager &operator=(const PhysicsManager& m);
	PhysicsManager(const PhysicsManager&m);
};
#endif //_PHYSICS_MANAGER_H