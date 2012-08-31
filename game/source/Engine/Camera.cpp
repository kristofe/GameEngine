#include "Game.h"
#include "Platform.h"
#include "Camera.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "RenderTarget.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "LayerManager.h"

////////////////////////////////////////////////////////////////////////////////
Camera::Camera(){
	mIsMain = true;
	mIsOrtho = true;
	mLeft = 0.0f;
	mRight = 480.0f;
	mBottom = 0.0f;
	mTop = 320.0f;
	mNear = -500.0f;
	mFar = 500.0f;
	mRenderTarget = NULL;
}
////////////////////////////////////////////////////////////////////////////////
Camera::Camera(PersistentData& storageData):SceneObject(storageData)
{
	mRenderTarget = NULL;
	std::string layerMaskName = storageData.GetProperty("LayerMask");
	mLayerMaskID = LayerManager::GetInstance().GetLayerID(layerMaskName);
	
	mIsMain = storageData.GetPropertyB("IsMain");
	PersistentData* node = storageData.GetNodeFromPath("RenderTarget");
	if(node != NULL)
		mRenderTargetTag = node->GetProperty("Tag");
	node = storageData.GetNodeFromPath("Viewport");
	if(node != NULL)
	{
		mViewport.x = node->GetPropertyF("x");
		mViewport.y = node->GetPropertyF("y");
		mViewport.width = node->GetPropertyF("Width");
		mViewport.height = node->GetPropertyF("Height");
	}
	
	node = storageData.GetNodeFromPath("OrthographicProjection");
	if(node != NULL)
	{
		mLeft = node->GetPropertyF("Left");
		mRight = node->GetPropertyF("Right");
		mBottom = node->GetPropertyF("Bottom");
		mTop = node->GetPropertyF("Top");
		mNear = node->GetPropertyF("Near");
		mFar = node->GetPropertyF("Far");
		CreateOrtho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	
	node = storageData.GetNodeFromPath("PerspectiveProjection");
	if(node != NULL)
	{
		float fov = node->GetPropertyF("FOV");
		float aspect = node->GetPropertyF("Aspect");
		mNear = node->GetPropertyF("Near");
		mFar = node->GetPropertyF("Far");
		CreatePerspective(fov, aspect, mNear, mFar);
	}
}
////////////////////////////////////////////////////////////////////////////////
Camera::~Camera(){
	
}
////////////////////////////////////////////////////////////////////////////////
void Camera::LevelLoaded(){
	GameObject* go = mGame.GetTaggedObject(mRenderTargetTag);
	if(go)
	{
		mRenderTarget = static_cast<RenderTarget*>(go);	
	}
	//Could sync the Viewports here
	//mViewport.width = mRenderTarget->mWidth;
	//mViewport.height = mRenderTarget->mHeight;
	
}
////////////////////////////////////////////////////////////////////////////////
void Camera::SetViewport(float x, float y, float width, float height)
{
	mViewport.x = x;
	mViewport.y = y;
	mViewport.width = width;
	mViewport.height = height;
	//glViewport(0, 0, (GLsizei)rt->GetWidth(), (GLsizei)rt->GetHeight());
	
}
////////////////////////////////////////////////////////////////////////////////
void Camera::CreateOrtho(float left, float right, float bottom, float top, float near, float far)
{
	mIsOrtho = true;
	mLeft = left;
	mRight = right;
	mBottom = bottom;
	mTop = top;
	mNear = near;
	mFar = far;
	
	//glOrtho(0, rt->GetWidth(), 0, rt->GetHeight(),-500,500);
}

////////////////////////////////////////////////////////////////////////////////
void Camera::CreatePerspective(float fovy, float aspect, float zNear, float zFar)
{
	mIsOrtho = false;
	mTop		= zNear * tan(fovy * M_PI / 360.0f);
	mBottom		= -mTop;
	mLeft		= mBottom * aspect;
	mRight		= mTop * aspect;
	
	mNear = zNear;
	mFar = zFar;
	
	//glFrustum(mLeft, mRight, mBottom, mTop, mNear, mFar);
	
	
}
/////////////////////////////////////////////////////////////////////////////////