/*
 *  PhysicsManager.cpp
 *  iPhone
 *
 *  Created by Kristofer Schlachter on 8/8/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "Game.h"
#include "PhysicsManager.h"

PhysicsProxy::PhysicsProxy(SceneObject* so, ColliderID cID, bool isRigidBody, bool isTrigger, float pMass) : mSceneObject(*so),mCollisionShapeID(cID),mRigidBodyID(-1),mIsRigidBody(isRigidBody),mIsTrigger(isTrigger),mMass(pMass)
{

	btCollisionShape* s = Game::GetInstance().GetPhysicsManager().GetCollisionShape(mCollisionShapeID);
	if(s != NULL)
	{
		s->setUserPointer(this);
	}
	if(mIsTrigger)
	{
		mIsRigidBody = true;
	}
	
	if(mIsRigidBody)
		Game::GetInstance().GetPhysicsManager().ConstructAndAssignRigidBody(*this);
}
void  PhysicsProxy::OnCollision(PhysicsProxy& other, const btVector3& contactPointWorldCoord, const btVector3& normal)
{
	IvVector3 cp;
	cp.Set(contactPointWorldCoord.x(), contactPointWorldCoord.y(), contactPointWorldCoord.z());
	IvVector3 n;
	n.Set(normal.x(), normal.y(), normal.z());
	if(mIsTrigger)
	{
		std::map<RigidBodyID,unsigned int>::iterator it = mTriggerOccupants.find(other.mRigidBodyID);
		if(it == mTriggerOccupants.end())
		{
			mSceneObject.OnTriggerEnter(other,cp,n);
			Game::GetInstance().GetPhysicsManager().TrackTrigger(this);
		}
		else 
		{
			mSceneObject.OnTriggerStay(other,cp,n);
		}

		mTriggerOccupants[other.mRigidBodyID] = Time::computerTimeMillis;
		
		
	}
	else if(other.mIsTrigger == false)
		mSceneObject.OnCollision(other,cp,n); 
	
	
}

void PhysicsProxy::UpdateTriggerOccupants()
{
	std::map<RigidBodyID,unsigned int>::iterator it = mTriggerOccupants.begin();
	while(it != mTriggerOccupants.end())
	{
		RigidBodyID rbID = it->first;
		unsigned int timeStamp = it->second;
		btRigidBody* rb = Game::GetInstance().GetPhysicsManager().GetRigidBody(rbID);
		if(rb != NULL)
		{
			
			if(Time::computerTimeMillis > timeStamp)
			{
				mTriggerOccupants.erase(it);
				PhysicsProxy* proxy = (PhysicsProxy*)rb->getUserPointer();
				mSceneObject.OnTriggerLeave(*proxy);
				Game::GetInstance().GetPhysicsManager().StopTrackingTrigger(this);

			}
		}
		++it;
	}
	
}

PhysicsManager::PhysicsManager(Game& game):mGame(game){
	mPaused = true;
	
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mDispatcher = new	btCollisionDispatcher(mCollisionConfiguration);
	
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	mOverlappingPairCache = new btDbvtBroadphase();
	
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = new btSequentialImpulseConstraintSolver;
	
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher,mOverlappingPairCache,mSolver,mCollisionConfiguration);
	
	mDynamicsWorld->setGravity(btVector3(0,-10,0));	
}

PhysicsManager::~PhysicsManager(){
	//cleanup in the reverse order of creation/initialization
	
	//remove the rigidbodies from the dynamics world and delete them
	for (int i=mDynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		mDynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}
	
	//delete collision shapes
	for (int j=0;j<mCollisionShapes.size();j++)
	{
		btCollisionShape* shape = mCollisionShapes[j];
		mCollisionShapes[j] = 0;
		delete shape;
	}
	
	//delete dynamics world
	delete mDynamicsWorld;
	
	//delete mSolver
	delete mSolver;
	
	//delete broadphase
	delete mOverlappingPairCache;
	
	//delete dispatcher
	delete mDispatcher;
	
	delete mCollisionConfiguration;
	
	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	mCollisionShapes.clear();
	mRigidBodies.clear();
}

void PhysicsManager::SetGravity(float x, float y, float z){
	mDynamicsWorld->setGravity(btVector3(x,y,z));	
	
}

ColliderID PhysicsManager::ConstructCollider(PersistentData& storageData)
{
	ColliderID id = -1;
	std::string type = storageData.GetProperty("type");
	if(type == "BOX")
	{
		if(storageData.GetNodeFromPath("Extents") != NULL)
		{
			IvVector3 extents = storageData.GetVector3Child("Extents");
			id = ConstructBoxCollider(extents);
		}
	}
	else if(type == "SPHERE")
	{
		if(storageData.GetNodeFromPath("Radius") != NULL)
		{
			float radius = storageData.GetNodeFromPath("Radius")->GetPropertyB("value");
			id = ConstructSphereCollider(radius);
		}
	}
	return id;
}

ColliderID PhysicsManager::ConstructBoxCollider(const IvVector3& extents)
{
	btCollisionShape* collisionProxy = new btBoxShape(btVector3(btScalar(extents.x),btScalar(extents.y),btScalar(extents.z)));
	mCollisionShapes.push_back(collisionProxy);
	return mCollisionShapes.size() -1;
	
}

ColliderID PhysicsManager::ConstructSphereCollider(float radius)
{
	btCollisionShape* collisionProxy = new btSphereShape(btScalar(radius));
	mCollisionShapes.push_back(collisionProxy);
	return mCollisionShapes.size() -1;
	
}

RigidBodyID PhysicsManager::ConstructRigidBody(PhysicsProxy* pProxy, ColliderID pColliderID, float pMass)
{
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (pMass != 0.f);
	btCollisionShape* collisionProxy = GetCollisionShape(pColliderID);
	if(collisionProxy == NULL)
		return -1;
	
	
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		collisionProxy->calculateLocalInertia(pMass,localInertia);
	
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(pMass,pProxy,collisionProxy,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setUserPointer(pProxy);
	if(pProxy->mIsTrigger)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	//add the body to the dynamics world
	mDynamicsWorld->addRigidBody(body);	
	
	RigidBodyID rbID = mRigidBodies.size();
	mRigidBodies.push_back(body);
	return rbID;
}

void PhysicsManager::ConstructAndAssignRigidBody(PhysicsProxy& pProxy)
{
	pProxy.mRigidBodyID = ConstructRigidBody(&pProxy, pProxy.mCollisionShapeID, pProxy.mMass);
}

void PhysicsManager::Start(unsigned int time){
	 mPaused = false;
	 
	return;
	 ///create a few basic rigid bodies
	 btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
	 
	 
	 
	 mCollisionShapes.push_back(groundShape);
	 
	 btTransform groundTransform;
	 groundTransform.setIdentity();
	 groundTransform.setOrigin(btVector3(0,56,0));
	 
	 {
		 btScalar mass(0.);
		 
		 //rigidbody is dynamic if and only if mass is non zero, otherwise static
		 bool isDynamic = (mass != 0.f);
		 
		 btVector3 localInertia(0,0,0);
		 if (isDynamic)
			 groundShape->calculateLocalInertia(mass,localInertia);
		 
		 //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		 btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		 btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		 btRigidBody* body = new btRigidBody(rbInfo);
		 
		 //add the body to the dynamics world
		 mDynamicsWorld->addRigidBody(body);
	 }
	 
	 
	 {
		 //create a dynamic rigidbody
		 
		 //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		 btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		 mCollisionShapes.push_back(colShape);
		 
		 /// Create Dynamic Objects
		 btTransform startTransform;
		 startTransform.setIdentity();
		 
		 btScalar	mass(1.f);
		 
		 //rigidbody is dynamic if and only if mass is non zero, otherwise static
		 bool isDynamic = (mass != 0.f);
		 
		 btVector3 localInertia(0,0,0);
		 if (isDynamic)
			 colShape->calculateLocalInertia(mass,localInertia);
		 
		 startTransform.setOrigin(btVector3(2,10,0));
		 
		 //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		 btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		 btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
		 btRigidBody* body = new btRigidBody(rbInfo);
		 
		 mDynamicsWorld->addRigidBody(body);
	 }
 }
 
/*
 void PhysicsManager::Stop(){
	mPaused = true;
}

void PhysicsManager::Pause(){
	mPaused = true;
}

void PhysicsManager::Unpause(){
	mPaused = false;
}
*/
void PhysicsManager::Update(){
	
	//mDynamicsWorld->stepSimulation(1.f/60.f,10);//This should be updated to the actual time that has elapsed;
	mDynamicsWorld->stepSimulation(Time::deltaTime, 10);
	ProcessCollisions();
	ProcessTriggers();
	/*
	//print positions of all objects
	for (int j=mDynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
	{
		btCollisionObject* obj = mDynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			btTransform trans;
			body->getMotionState()->getWorldTransform(trans);
			printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
		}
	}
	 */

	
}

void PhysicsManager::ProcessCollisions()
{
	//Assume world->stepSimulation or world->performDiscreteCollisionDetection has been called
	
	int numManifolds = mDynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
		
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
				
				PhysicsProxy* objA = (PhysicsProxy*)obA->getUserPointer();
				PhysicsProxy* objB = (PhysicsProxy*)obB->getUserPointer();
				if(objA != NULL)
				{
					objA->OnCollision(*objB, ptA, -normalOnB);
				}
				if(objB != NULL)
				{
					objB->OnCollision(*objA, ptB, normalOnB);
				}
			}
		}
	}	
}

void  PhysicsManager::ProcessTriggers()
{
	//Cleanup any empty triggers
	for(unsigned int i = 0; i < mPendingTriggerDeletions.size(); i++)
	{
		mActiveTriggers.erase(mPendingTriggerDeletions[i]);
	}
	mPendingTriggerDeletions.clear();
	
	std::set<PhysicsProxy*>::iterator it = mActiveTriggers.begin();
	while(it != mActiveTriggers.end())
	{
		PhysicsProxy* proxy = *it;
		proxy->UpdateTriggerOccupants();
		++it;
	}
	
}

void  PhysicsManager::StopTrackingTrigger(PhysicsProxy* proxy)
{
	mPendingTriggerDeletions.push_back(proxy);
	
}

void  PhysicsManager::TrackTrigger(PhysicsProxy* proxy)
{
	mActiveTriggers.insert(proxy);
}


