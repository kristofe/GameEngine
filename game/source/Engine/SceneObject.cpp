/*
 *  SceneObject.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SceneObject.h"
#include "Game.h"

////////////////////////////////////////////////////////////////////////////////
SceneObject::SceneObject():mParent(NULL),mRoot(NULL),mPhysicsProxy(NULL){
	mMutex = Threading::MutexInit();
	mLocalPosition.Set(0,0,0);
	mLocalRotation.Set(0,0,0);
	mLocalScale.Set(1,1,1);
	
}
////////////////////////////////////////////////////////////////////////////////
SceneObject::SceneObject(PersistentData& storageData):GameObject(storageData),mParent(NULL),mRoot(NULL),mPhysicsProxy(NULL){
	mMutex = Threading::MutexInit();
	mLocalPosition.Set(0,0,0);
	mLocalRotation.Set(0,0,0);
	mLocalScale.Set(1,1,1);
	IvVector3 pos, scale, rot;

	
	if(storageData.GetNodeFromPath("Position") != NULL)
	{
		pos = storageData.GetVector3Child("Position");
		Translate(pos);
	}
	if(storageData.GetNodeFromPath("Rotation") != NULL)
	{
		rot = storageData.GetVector3Child("Rotation");
		Rotate(rot.x,rot.y,rot.z);
	}
	if(storageData.GetNodeFromPath("Scale") != NULL)
	{
		scale = storageData.GetVector3Child("Scale");
		Scale(scale);
	}
	/*<Physics>
	 <Collider type="BOX">
	 <Extents x="10" y="10" z="10"/>
	 </Collider>
	 <RigidBody mass="1.0"/>
	 </Physics>*/
	
	//ColliderID cID = mGame.GetPhysicsManager().ConstructBoxCollider(IvVector3(25.0f,35.0f,5.0f));
	//mPhysicsProxy = new PhysicsProxy((SceneObject&)*this,cID,true, 1.0f);
	
	PersistentData* phys = storageData.GetNodeFromPath("Physics");
	if(phys != NULL)
	{
		ColliderID cID = -1;
		float mass = 0.0f;
		bool makeRigidBody = false;
		bool isTrigger = false;
		PersistentData* collider = phys->GetNodeFromPath("Collider");
		if(collider!= NULL)
		{
			cID = mGame.GetPhysicsManager().ConstructCollider(*collider);
			isTrigger = collider->GetPropertyB("isTrigger");
		}
		if(phys->GetNodeFromPath("RigidBody") != NULL)
		{
			makeRigidBody = true;
			mass = phys->GetNodeFromPath("RigidBody")->GetPropertyB("mass");
		}
		if(cID >= 0)
		{
			mPhysicsProxy = new PhysicsProxy((SceneObject*)this,cID,makeRigidBody,isTrigger,mass);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
SceneObject::~SceneObject(){
	Threading::MutexDestroy(mMutex);
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::AddedToDeletionList()
{
	std::vector<SceneObject*>::iterator it = mChildren.begin();
	while(it != mChildren.end()){
		mGame.DeleteGameObject((*it)->GetID());
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("SceneObject");
	
	storageData.AddVector3Child(mLocalPosition,"Position");
	storageData.AddQuatChild(mLocalRotation,"Rotation");
	storageData.AddVector3Child(mLocalScale,"Scale");
	
	char buff[128];
	unsigned int objID =  (unsigned int)(mRoot != NULL?mRoot->GetID():0);
	
	sprintf(buff, "%d",objID);
	storageData.SetProperty("Root",buff);
	
	objID = (unsigned int)(mParent != NULL?mParent->GetID():0);
	sprintf(buff, "%d",objID);
	storageData.SetProperty("Parent",buff);
	PersistentData* children = new PersistentData("Children");
	storageData.AddChild(children);
	
	for(int i = 0; i < (int)mChildren.size(); ++i)
	{
		PersistentData* c = new PersistentData("Child");
		objID = static_cast<unsigned int>(mChildren[i]->GetID());
		sprintf(buff, "%d",objID);
		std::string s = buff;
		c->SetTextProperty(s);
		children->AddChild(c);
		
	}
	
	storageData.AddMatrix44Child(mLocalTransform,"Transform");
	
	
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	mLocalPosition = storageData.GetVector3Child("Position");
	mLocalRotation = storageData.GetQuatChild("Rotation");
	mLocalScale = storageData.GetVector3Child("Scale");
	

	mID = static_cast<GameID>(storageData.GetPropertyU("Root"));
	mOwner = static_cast<GameID>(storageData.GetPropertyU("Owner"));
	std::list<PersistentData*> children;
	storageData.GetNodesFromPath("Children",children);
	std::list<PersistentData*>::iterator it = children.begin();
	while(it != children.end())
	{
		PersistentData* c = (*it);
		GameID id = static_cast<GameID> (c->GetTextAsUInt());
		//mGame Better Not Be NULL At This Point!!!
		SceneObject* so = dynamic_cast<SceneObject*>(mGame.GetGameObject(id));
		mChildren.push_back(so);
		++it;
	}
	
	IvMatrix44 mtx = storageData.GetMatrix44Child("Transform");
	
	if(!mtx.IsIdentity())
	{
		mLocalTransform = mtx;
		UpdateFromNewLocalTransform_NoMutex();
	}
	else
	{
		Translate(mLocalPosition);
		Rotate(mLocalRotation);
		Scale(mLocalScale);
		
		
	}
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::SetLocalTransform(const IvMatrix44& m){
	Threading::MutexLock(mMutex);
	mDirty = true;
	//UpdateOrientationAndPositionData();
	mLocalTransform = m;
	
	//Now update all of the other transforms
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);
	
	
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::UpdateChildren(){
	std::vector<SceneObject*>::iterator it = mChildren.begin();
	while(it != mChildren.end()){
		Threading::MutexLock(mMutex);
		(*it)->UpdateFromParentWorldTransform_NoMutex(mWorldTransform);
		(*it)->UpdateDirectionAndPositionVectors_NoMutex();
		Threading::MutexUnlock(mMutex);
		(*it)->UpdateChildren();
		++it;
	}

	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::UpdateFromParentWorldTransform_NoMutex(const IvMatrix44& parentWorldTransform){
	mWorldTransform = parentWorldTransform * mLocalTransform;
	mWorldInverseTransform = AffineInverse(mWorldTransform);
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::UpdateDirectionAndPositionVectors_NoMutex(){
	mLocalTransform.GetColumns(mLocalRight,mLocalUp, mLocalForward,mLocalPosition);
	mWorldTransform.GetColumns(mRight,mUp, mForward,mPosition);
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::UpdateFromNewLocalTransform_NoMutex(){
	mLocalInverseTransform = AffineInverse(mLocalTransform);
	if(mParent != NULL){
		IvMatrix44 parentTransform;
		mParent->GetWorldTransform(parentTransform);
		UpdateFromParentWorldTransform_NoMutex(parentTransform);
	}else{
		mWorldTransform = mLocalTransform;
		mWorldInverseTransform = AffineInverse(mWorldTransform);
	}
	
	UpdateDirectionAndPositionVectors_NoMutex();
	UpdateChildren();
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::Translate(const IvVector3& p){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvMatrix44 m;
	m.Translation(p);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::SetPosition(const IvVector3& p){
	Threading::MutexLock(mMutex);
	mDirty = true;

	mLocalTransform.SetPosition(p);
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::SetWorldPosition(const IvVector3& worldPos){
	Threading::MutexLock(mMutex);
	mDirty = true;

	if(mParent == NULL)
	{
		mLocalTransform.SetPosition(worldPos);
	}
	else
	{
		//put the new position into the parent's coordinate frame
		IvMatrix44 mtx;		
		mParent->GetWorldInverseTransform(mtx);
		
		IvVector3 pos = mtx.TransformPoint(worldPos);
		mLocalTransform.SetPosition(pos);
		
	}
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);
}


////////////////////////////////////////////////////////////////////////////////
void SceneObject::SetRoot(SceneObject* r){
	Threading::MutexLock(mMutex);
	mDirty = true;
	mRoot = r;
	
	Threading::MutexUnlock(mMutex);
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::SetParent(SceneObject* p){
	//Physics system uses world transfroms so this cannot be a child of anything if it is part of the physics system
	if(mPhysicsProxy != NULL && p != NULL)
	{
		printf("Physics system uses world transfroms so this SceneObject (ID=%d) cannot be a child of anything if it is part of the physics system", mID);
		return;
	}
	Threading::MutexLock(mMutex);
	mDirty = true;
	mParent = p;
	IvMatrix44 parentWorldTransform;
	if(p != NULL){
		mRoot = p->GetRoot();
		mParent->GetWorldTransform(parentWorldTransform);
	}else{
		mRoot = NULL;
	}
	UpdateFromParentWorldTransform_NoMutex(parentWorldTransform);
	UpdateDirectionAndPositionVectors_NoMutex();
	Threading::MutexUnlock(mMutex);	
	
	//if(p != NULL)
	//	p->UpdateChildren();
	
	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::AddChild(SceneObject* t){
	Threading::MutexLock(mMutex);
	mDirty = true;
	mChildren.push_back(t);
	Threading::MutexUnlock(mMutex);
	
	t->SetParent(this);
		
}

////////////////////////////////////////////////////////////////////////////////
bool SceneObject::HasChild(SceneObject* t){
	for(unsigned int i = 0; i < mChildren.size(); ++i){
		SceneObject* obj = mChildren[i];
		if(obj == t){
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::RemoveChild(SceneObject* t){
	
	
	bool found = false;
	std::vector<SceneObject*>::iterator it = mChildren.begin();
	while(it != mChildren.end()){
		if(*it == t){
			found = true;
			break;
		}
		++it;
	}
	if(found && it != mChildren.end()){
		Threading::MutexLock(mMutex);
		mDirty = true;
		(*it)->SetParent(NULL);
		mChildren.erase(it);
		Threading::MutexUnlock(mMutex);
		
	}
	
}


////////////////////////////////////////////////////////////////////////////////
unsigned int SceneObject::GetNumChildren(){
	return (unsigned int)mChildren.size();
}

////////////////////////////////////////////////////////////////////////////////
bool SceneObject::IsChildOf(SceneObject* t){
	//Have to traverse the tree!
	std::vector<SceneObject*>::iterator it = mChildren.begin();
	while(it != mChildren.end()){
		if(*it == t){
			return true;
		}else{
			return (*it)->IsChildOf(t);
		}
		++it;
	}
	return false;
}


 //Need Multiple Of the following
////////////////////////////////////////////////////////////////////////////////
void SceneObject::Rotate(IvMatrix33& rot){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvQuat r(rot);
	mLocalRotation = r;
	IvMatrix44 m;
	m.Rotation(rot);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::Rotate(IvQuat& rot){
	Threading::MutexLock(mMutex);
	mDirty = true;
	mLocalRotation = rot;
	IvMatrix44 m;
	m.Rotation(rot);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}
	
////////////////////////////////////////////////////////////////////////////////
void SceneObject::Rotate(IvVector3& axis, float angle){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvQuat r(axis, angle);
	mLocalRotation = r;
	IvMatrix44 m;
	m.Rotation(axis,angle);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::Rotate(float x_angle, float y_angle, float z_angle){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvQuat r;
	r.Set(x_angle,y_angle,z_angle);
	mLocalRotation = r;
	IvMatrix44 m;
	m.Rotation(x_angle, y_angle, z_angle);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::RotateX(float angle){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvVector3 axis = IvVector3::xAxis;
	IvQuat r(axis, angle);
	mLocalRotation = r;
	IvMatrix44 m;
	m.RotationX(angle);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::RotateY(float angle){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvVector3 axis = IvVector3::yAxis;
	IvQuat r(axis, angle);
	mLocalRotation = r;
	IvMatrix44 m;
	m.RotationY(angle);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::RotateZ(float angle){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvVector3 axis = IvVector3::zAxis;
	IvQuat r(axis, angle);
	mLocalRotation = r;
	IvMatrix44 m;
	m.RotationZ(angle);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);	
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::Scale(const IvVector3& scale){
	Threading::MutexLock(mMutex);
	mDirty = true;
	mLocalScale = scale;
	IvMatrix44 m;
	m.Scaling(scale);
	mLocalTransform *= m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);		
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::ResetLocalTransform(){
	Threading::MutexLock(mMutex);
	mDirty = true;
	mLocalScale.x = mLocalScale.y = mLocalScale.z = 1.0f;
	mLocalTransform.Identity();
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);		
}

////////////////////////////////////////////////////////////////////////////////
void SceneObject::LookAt(IvVector3& position, IvVector3& targetPoint, IvVector3& up){
	Threading::MutexLock(mMutex);
	mDirty = true;
	IvMatrix44 m;
	
	IvVector3 dir3 = targetPoint - position;
	dir3.Normalize();
	
	IvVector3 right3 = dir3.Cross(up);
	
	
	IvVector4 upAxis(up);
	IvVector4 fwdAxis(dir3);
	IvVector4 rightAxis(right3);
	IvVector4 pos(position,1);
	
	
	m.SetColumns(rightAxis,upAxis,fwdAxis,pos);

	mLocalTransform = m;
	UpdateFromNewLocalTransform_NoMutex();
	
	Threading::MutexUnlock(mMutex);
}
////////////////////////////////////////////////////////////////////////////////
void	SceneObject::OnCollision(PhysicsProxy& other, IvVector3& contactPointWorldCoord, IvVector3& normal)
{
	//printf("Collision %d-%d, at (%3.2f,%3.2f,%3.f) direction (%3.2f,%3.2f,%3.f)\n",mID, other.mSceneObject.mID, contactPointWorldCoord.x,contactPointWorldCoord.y,contactPointWorldCoord.z, normal.x, normal.y, normal.z);
}
////////////////////////////////////////////////////////////////////////////////
void	SceneObject::OnTriggerEnter(PhysicsProxy& other, IvVector3& contactPointWorldCoord, IvVector3& normal)
{
	//printf("Trigger Enter %d-%d, at (%3.2f,%3.2f,%3.f) direction (%3.2f,%3.2f,%3.f)\n",mID, other.mSceneObject.mID, contactPointWorldCoord.x,contactPointWorldCoord.y,contactPointWorldCoord.z, normal.x, normal.y, normal.z);
}
////////////////////////////////////////////////////////////////////////////////
void	SceneObject::OnTriggerStay(PhysicsProxy& other, IvVector3& contactPointWorldCoord, IvVector3& normal)
{
	//printf("TriggerStay ");
	//printf("Collision %d-%d, at (%3.2f,%3.2f,%3.f) direction (%3.2f,%3.2f,%3.f)\n",mID, other.mSceneObject.mID, contactPointWorldCoord.x,contactPointWorldCoord.y,contactPointWorldCoord.z, normal.x, normal.y, normal.z);
}
////////////////////////////////////////////////////////////////////////////////
void	SceneObject::OnTriggerLeave(PhysicsProxy& other)
{
	//printf("\n\nTrigger Left\n");
	//printf("Collision %d-%d, at (%3.2f,%3.2f,%3.f) direction (%3.2f,%3.2f,%3.f)\n",mID, other.mSceneObject.mID, contactPointWorldCoord.x,contactPointWorldCoord.y,contactPointWorldCoord.z, normal.x, normal.y, normal.z);
}
////////////////////////////////////////////////////////////////////////////////
/*void SceneObject::AddCollider(ColliderID colliderID)
{
	if(mPhysicsProxy == NULL)
	{
		mPhysicsProxy = new PhysicsProxy(this, Game::GetInstance().GetPhysicsManager());
	}
	mPhysicsProxy->mCollisionShapeID = colliderID
	mPhysicsProxy->mMass = 0.0f;
	
}
////////////////////////////////////////////////////////////////////////////////
void SceneObject::AddRigidBody(RigidBodyID rbID, ColliderID colliderID, float mass)
{
	if(mPhysicsProxy == NULL)
	{
		mPhysicsProxy = new PhysicsProxy(this, Game::GetInstance().GetPhysicsManager());
	}
	mPhysicsProxy->mRigidBodyID = rbID;
	mPhysicsProxy->mCollisionShapeID = colliderID
	mPhysicsProxy->mMass = mass;
}*/
////////////////////////////////////////////////////////////////////////////////

 