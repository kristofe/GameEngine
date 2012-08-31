#ifndef _CAMERA_H
#define _CAMERA_H

#include "RenderObject.h"
#include "Rect.h"

class RenderTarget;

class Camera : public SceneObject{
public:
	Camera();
	Camera(PersistentData& storageData);
	~Camera();
	
public:
	bool		mIsMain;
	bool		mIsOrtho;

	Rect		mViewport;
	float		mLeft;
	float		mRight;
	float		mTop;
	float		mBottom;
	
	float		mNear;
	float		mFar;
	
	int			mLayerMaskID;
protected:
	std::string		mRenderTargetTag;
	RenderTarget*	mRenderTarget;

private:
	//Remove Copy Constructor and assignment operator
	Camera		&operator =  (const Camera &m) { printf("Camera = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	Camera(const Camera &m){printf("Camera COPY CTOR SHOULDN'T BE GETTING CALLED");};
public:
	void Init();
	void LevelLoaded();
	
	virtual std::string GetTypeString()
	{
		return "Camera";
	}
	
	void CreateOrtho(float left, float right, float bottom, float top, float near, float far);
	void CreatePerspective(float fovy,float aspect, float znear, float zfar);
	void SetViewport(float x, float y, float width, float height);
	RenderTarget* GetRenderTarget(){return mRenderTarget;};
	
};

#endif //_RENDER_PLANE_H