#include "RenderTarget.h"
#include "LetterDisplayAnimation.h"
#include "WordProgressDisplay.h"
#include "LetterChooser.h"
#include "Game.h"
#include "GameLogic.h"
#include "Platform.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "Scene.h"

////////////////////////////////////////////////////////////////////////////////
RenderTarget::RenderTarget(PersistentData& storageData):SceneObject(storageData)
{
	//mObjectVectors[Renderer::OpaqueObjectsArray] = & mOpaqueRenderObjects;
	//mObjectVectors[Renderer::TransparentObjectsArray] = & mTranparentRenderObjects;
	
	mWidth = storageData.GetPropertyU("width");
	mHeight = storageData.GetPropertyU("height");
	mOffsetX = storageData.GetPropertyU("offsetX");
	mOffsetY = storageData.GetPropertyU("offsetY");
	mHasAlpha = storageData.GetPropertyB("hasAlpha");
	//mRenderEverything = storageData.GetPropertyB("RenderEverything");
	//mCameraTag = storageData.GetProperty("Camera");
	/*if(!mRenderEverything)
	{
		std::list<PersistentData*> renderObjects = storageData.GetNodeFromPath("RenderObjects")->GetChildren();
		std::list<PersistentData*>::iterator it = renderObjects.begin();
		while(it != renderObjects.end())
		{
			mRenderObjectTags.push_back((*it)->GetProperty("Tag"));	
			mExclusiveToRenderTarget.push_back((*it)->GetPropertyB("ExclusiveToTarget"));
			++it;
		}
		
	}*/
	char name[512];
	sprintf(name,"RenderTarget_Tag_%s_%d",mTag.c_str(),mID);
	mTextureID = RendererObjectFactory::CreateTexture(name,name);
	
	
	mTargetTexture = &(RendererObjectFactory::GetTexture(mTextureID));
	mTargetTexture->mWidth = mWidth;
	mTargetTexture->mHeight = mHeight;
	mTargetTexture->mHasAlpha = mHasAlpha;
	mTargetTexture->mDataLength = mWidth*mHeight*4;
	mTargetTexture->mNumberOfColors = 4;
	mTargetTexture->mBitsPerPixel = 32;
	mTargetTexture->mPixelData = (unsigned int*)calloc(mTargetTexture->mDataLength, sizeof(unsigned int));
	
	mGame.GetRenderer().Retain(mID,mTextureID);
	Game::GetInstance().GetRenderer().AddRenderTarget(this);
	/*if(mHasAlpha)
	{
		mTargetTexture->mPixelFormat = PIXELFORMAT_RGBA;
	}
	else
	{
		mTargetTexture->mPixelFormat = PIXELFORMAT_RGB;
	}*/
	
}
////////////////////////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
	mGame.GetRenderer().Release(mID,mTextureID);
}
////////////////////////////////////////////////////////////////////////////////
void RenderTarget::LevelLoaded()
{
	/*for(unsigned int i = 0; i < mRenderObjectTags.size(); ++i)
	{
		
		RenderObject* ro = (RenderObject*)Game::GetInstance().GetTaggedObject(mRenderObjectTags[i]);
		ro->SetExclusiveToRenderTarget(mExclusiveToRenderTarget[i]);
		if(ro->IsTransparent())
			mTranparentRenderObjects.push_back(ro);
		else
			mOpaqueRenderObjects.push_back(ro);	
		
	}*/

}
////////////////////////////////////////////////////////////////////////////////
void RenderTarget::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("RenderTarget");
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderTarget::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
/*
////////////////////////////////////////////////////////////////////////////////
void RenderTarget::AddRenderObject(RenderObject* ro)
{
	if(ro->IsTransparent())
		mTranparentRenderObjects.push_back(ro);
	else
		mOpaqueRenderObjects.push_back(ro);
	 
}
////////////////////////////////////////////////////////////////////////////////
void RenderTarget::RemoveRenderObject(RenderObject* ro)
{
	
	 for(int idx = 0; idx < Renderer::ObjectsArrayCount; idx++){
		RenderObjectVectorIt it = mObjectVectors[idx]->begin();
		while(it != mObjectVectors[idx]->end())
		{
			if(*it == ro)
			{
				mObjectVectors[idx]->erase(it);
				break;
			}
			++it;
		}
	}
	 
}
 */
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////