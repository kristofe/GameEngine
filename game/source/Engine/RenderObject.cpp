/*
 *  RenderObject.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 2/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "Game.h"
#include "RenderObject.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

////////////////////////////////////////////////////////////////////////////////
RenderObject::RenderObject():mIsTrasparent(true){
	mGame.GetRenderer().AddRenderObject(mID);
	
}

////////////////////////////////////////////////////////////////////////////////
RenderObject::RenderObject(PersistentData& storageData):SceneObject(storageData),mIsTrasparent(true){
	mGame.GetRenderer().AddRenderObject(mID);
	mVisible = !storageData.GetPropertyB("Hidden");
	mIsTrasparent = storageData.GetPropertyB("IsTransparent");
	mColor = storageData.GetColorChild("Color");
	//mExclusiveToRenderTarget = false;//storageData.GetPropertyB("ExclusiveToRenderTarget");
	mGame.GetRenderer().AddRenderObject(mID);
	

}
/*
////////////////////////////////////////////////////////////////////////////////
void RenderObject::PreSetup(PersistentData& storageData)
{
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::SetupMeshes(PersistentData& storageData)
{
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::SetupMaterialsAndTextures(PersistentData& storageData)
{
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::PostSetup(PersistentData& storageData)
{
	
}*/
////////////////////////////////////////////////////////////////////////////////
RenderObject::~RenderObject(){
	for(int i = 0; i < (int)mMeshes.size(); ++i)
	{
		mGame.GetRenderer().Release(mID,mMeshes[i]);
		Material& m = RendererObjectFactory::GetMaterial(mMaterials[i]);
		for(int j = 0; j < m.mTextureCount; j++)
		{
			mGame.GetRenderer().Release(mID,m.mTextureIDs[j]);
		}
		mGame.GetRenderer().Release(mID,mMaterials[i]);
	}
	mGame.GetRenderer().RemoveRenderObject(mID);
	mMaterials.clear();
	mMeshes.clear();

	mVisible = false;
	mActive = false;
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::Serialize(PersistentData& storageData)
{
	SceneObject::Serialize(storageData);
	storageData.SetName("RenderObject");
	
	char buff[128];
	unsigned int objID;

	PersistentData* meshes = new PersistentData("Meshes");
	storageData.AddChild(meshes);
	storageData.SetProperty("Visible",mVisible);
	//storageData.SetProperty("DisableZWrite",mDisableZWrite);
	
	for(int i = 0; i < (int)mMeshes.size(); ++i)
	{
		PersistentData* c = new PersistentData("Mesh");
		objID = static_cast<unsigned int>(mMeshes[i]);
		sprintf(buff, "%d",objID);
		std::string s = buff;
		c->SetTextProperty(s);
		meshes->AddChild(c);
	}

	PersistentData* materials = new PersistentData("Materials");
	storageData.AddChild(materials);
	
	for(int i = 0; i < (int)mMaterials.size(); ++i)
	{
		PersistentData* c = new PersistentData("Material");
		objID = static_cast<unsigned int>(mMaterials[i]);
		sprintf(buff, "%d",objID);
		std::string s = buff;
		c->SetTextProperty(s);
		materials->AddChild(c);
	}

	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::DeSerialize(PersistentData& storageData)
{
	SceneObject::DeSerialize(storageData);
	mVisible = !storageData.GetPropertyB("Hidden");
	//mDisableZWrite = storageData.GetPropertyB("DisableZWrite");

	PersistentData* c;
	GameID id;
	std::list<PersistentData*> children;
	std::list<PersistentData*>::iterator it;
	
	storageData.GetNodesFromPath("Meshes",children);
	it = children.begin();
	while(it != children.end())
	{
		c = (*it);
		id = static_cast<GameID> (c->GetTextAsUInt());
		mMeshes.push_back(id);
		//mGame.GetRenderer().Retain(mID,id);
		++it;
	}
	
	storageData.GetNodesFromPath("Materials",children);
	it = children.begin();
	while(it != children.end())
	{
		c = (*it);
		id = static_cast<GameID> (c->GetTextAsUInt());
		mMaterials.push_back(id);
		//mGame.GetRenderer().Retain(mID,id);
		++it;
	}
	
	mGame.GetRenderer().AddRenderObject(mID);
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::SetColor(const IvColor& color)
{

	for(unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		GameID meshID = mMeshes[i];
		Mesh& m = RendererObjectFactory::GetMesh(meshID);
		
		m.Lock();
		for(unsigned int i = 0; i < m.mColorCount; ++i)
		{
			m.mColors[i] = color;
		}
		
		m.Unlock();
	}


	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObject::SetUVs(const std::vector<IvVector2>& uvs)
{
	for(unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		GameID meshID = mMeshes[i];
		Mesh& m = RendererObjectFactory::GetMesh(meshID);
		
		m.Lock();
		for(unsigned int i = 0; i < m.mUVCount; ++i)
		{
			m.mUV[i] = uvs[i];
		}
		
		m.Unlock();
	}


	
}
////////////////////////////////////////////////////////////////////////////////

