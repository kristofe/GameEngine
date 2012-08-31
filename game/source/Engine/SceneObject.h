/*
 *  SceneObject.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SCENE_OBJECT_H
#define _SCENE_OBJECT_H

#include "GameObject.h"
#include <vector>



//THIS CLASS SHOULD BE RENDER THREAD SAFE!!!!

class Renderer;
class RendererObjectFactory;
class PhysicsProxy;

class SceneObject : public GameObject{
	friend class RendererObjectFactory;
	friend class Renderer;
	friend class RendererGL;
	friend class RendererGLES20;

protected:
	IvMatrix44					mLocalTransform;//The local matrix - independent of parent.
	IvMatrix44					mLocalInverseTransform;//The local matrix - independent of parent.
	IvMatrix44					mWorldTransform;//This is the product of all ancestors mLocalTransform's and this LocalTransform
	IvMatrix44					mWorldInverseTransform;//Inverse of mLocalToWorldTransform
	
	IvVector3					mPosition;
	IvVector3					mLocalPosition;

	IvVector3					mRight;
	IvVector3					mUp;
	IvVector3					mForward;
	
	IvVector3					mLocalRight;
	IvVector3					mLocalUp;
	IvVector3					mLocalForward;
	
	IvQuat						mLocalRotation;
	IvQuat						mWorldRotation;
	
	IvVector3					mLocalScale;
	
	
	
	//Have bounding information in here too
	
	
	SceneObject*					mRoot;//The topmost sceneobject this object has
	SceneObject*					mParent;
	std::vector<SceneObject*>		mChildren;

	PhysicsProxy*					mPhysicsProxy;
	
	bool						mDirty;
	Mutex*						mMutex;
	
	
public:
	SceneObject();
	SceneObject(PersistentData& storageData);
	~SceneObject();

	virtual void AddedToDeletionList();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	void	GetLocalTransform(IvMatrix44& m){ m= mLocalTransform;};
	void	SetLocalTransform(const IvMatrix44& m);
	void	GetLocalInverseTransform(IvMatrix44& m){ m = mLocalInverseTransform;};
	void	GetWorldTransform(IvMatrix44& m){ m = mWorldTransform;};
	void	GetWorldInverseTransform(IvMatrix44& m){ m = mWorldInverseTransform;};
	
	void	GetWorldPosition(IvVector3& v){v = mPosition;};
	
	void	GetLocalPosition(IvVector3& v){v = mLocalPosition;};
	
	virtual const float	GetCameraDistance() const {return mPosition.z;/*THIS IS NOT CORRECT... Need to multiply mPosition by inverse trans of camera!*/};
	
	void	GetTransformForPhysics(IvMatrix44& m){ m = mWorldTransform;};
	void	SetTransformFromPhysics(IvMatrix44& m) { SetLocalTransform(m);};
	
	void	GetRight(IvVector3& v){v =  mRight;};
	void	GetUp(IvVector3& v){v =  mUp;};
	void	GetForward(IvVector3& v){v = mForward;};
	
	void	GetLocalRotation(IvQuat& q){q = mLocalRotation;};
	void	GetRotation(IvQuat& q){q =  mWorldRotation;};
	
	void	GetLocalScale(IvVector3& v){v = mLocalScale;};
	
	SceneObject*		GetRoot(){return mRoot;};
	void				SetRoot(SceneObject* r);
	
	SceneObject*		GetParent(){return mParent;};
	void				SetParent(SceneObject* p);
	
	void				AddChild(SceneObject* t);
	bool				HasChild(SceneObject* t);
	void				RemoveChild(SceneObject* t);
	unsigned int		GetNumChildren();
	bool				IsChildOf(SceneObject* t);
	
	bool				IsDirty(){return mDirty;};
	
	
	void				Rotate(IvMatrix33& rot);
	void				Rotate(IvQuat& rot);
	void				Rotate(IvVector3& axis, float angle);
	void				Rotate(float x_angle, float y_angle, float z_angle);
	void				RotateX(float angle);
	void				RotateY(float angle);
	void				RotateZ(float angle);
	
	
	void				Translate(const IvVector3& p);
	void				SetPosition(const IvVector3& p);
	void				SetWorldPosition(const IvVector3& worldPos);
	
	void				Scale(const IvVector3& scale );
	 
	void				LookAt(IvVector3& position, IvVector3& targetPoint, IvVector3& up);
	 
	void				ResetLocalTransform();
	virtual void		OnCollision(PhysicsProxy& other, IvVector3& contactPointWorldCoord, IvVector3& normal);
	virtual void		OnTriggerEnter(PhysicsProxy& other, IvVector3& contactPointWorldCoord, IvVector3& normal);
	virtual void		OnTriggerStay(PhysicsProxy& other, IvVector3& contactPointWorldCoord, IvVector3& normal);
	virtual void		OnTriggerLeave(PhysicsProxy& other);
	//void				AddCollider(ColliderID colliderID);
	//void				AddRigidBody(RigidBodyID rbID, ColliderID colliderID, float mass = 0.0f);
	
	virtual std::string GetTypeString()
	{
		return "SceneObject";
	}
	
	
private:
	//Remove Copy Constructor and assignment operator
	SceneObject		&operator =  (const SceneObject &m) { printf("SceneObject = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	SceneObject(const SceneObject &m){printf("SceneObject COPY CTOR SHOULDN'T BE GETTING CALLED");};;

	
protected:
	void UpdateFromNewLocalTransform_NoMutex();
	void UpdateChildren();
	void UpdateFromParentWorldTransform_NoMutex(const IvMatrix44& parentWorldTransform);
	void UpdateDirectionAndPositionVectors_NoMutex();
	
};



/*
 IvMatrix44 uses 4 row vectors.   It is equivalent to the Document layout below.
 
 I1. Important note relating to OpenGl and this document
 -------------------------------------------------------
 
 In this document (as in most math textbooks), all matrices are drawn
 in the standard mathematical manner.  Unfortunately graphics libraries
 like IrisGL, OpenGL and SGI's Performer all represent them with the
 rows and columns swapped.
 
 Hence, in this document you will see (for example) a 4x4 Translation
 matrix represented as follows:
 
	 | 1  0  0  X |
	 |            |
	 | 0  1  0  Y |
 M = |            |
	 | 0  0  1  Z |
	 |            |
	 | 0  0  0  1 |
	 
 In Performer (for example) this would be populated as follows:
 
 M[0][1] = M[0][2] = M[0][3] =
 M[1][0] = M[1][2] = M[1][3] =
 M[2][0] = M[2][1] = M[2][3] = 0 ;
 M[0][0] = M[1][1] = M[2][2] = m[3][3] = 1 ;
 M[3][0] = X ;
 M[3][1] = Y ;
 M[3][2] = Z ;
 
 ie, the matrix is stored like this:
 
	 | M[0][0]  M[1][0]  M[2][0]  M[3][0] |
	 |                                    |
	 | M[0][1]  M[1][1]  M[2][1]  M[3][1] |
 M = |                                    |
	 | M[0][2]  M[1][2]  M[2][2]  M[3][2] |
	 |                                    |
	 | M[0][3]  M[1][3]  M[2][3]  M[3][3] |
	 
 OpenGL uses a one-dimensional array to store matrices - but fortunately,
 the packing order results in the same layout of bytes in memory - so
 taking the address of a pfMatrix and casting it to a float* will allow
 you to pass it directly into routines like glLoadMatrixf.
 
 In the code snippets scattered throughout this document, a one-dimensional
 array is used to store a matrix. The ordering of the array elements is
 transposed with respect to OpenGL.
 
 
 This Document                  OpenGL
 
	 | 0  1  2  3  |            | 0  4  8  12 |
	 |             |            |             |
	 | 4  5  6  7  |            | 1  5  9  13 |
 M = |             |        M = |             |
	 | 8  9  10 11 |            | 2  6  10 14 |
	 |             |            |             |
	 | 12 13 14 15 |            | 3  7  11 15 |
	 
 Q5.  How do matrices relate to coordinate systems?
 --------------------------------------------------
 With either 3x3 or 4x4 rotation, translation or shearing matrices, there
 is a simple relationship between each matrix and the resulting coordinate
 system.
 The first three columns of the matrix define the direction vector of the
 X, Y and Z axii respectively.
 If a 4x4 matrix is defined as:
 
		| A B C D |
 M =	| E F G H |
		| I J K L |
		| M N O P |
 
 Then the direction vector for each axis is as follows:
 
 X-axis = [ A E I ]
 Y-axis = [ B F J ]
 Z-axis = [ C G K ]
 
 
 */

#endif //_SCENE_OBJECT_H