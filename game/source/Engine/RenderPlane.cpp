#include "Game.h"
#include "Platform.h"
#include "RenderPlane.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

////////////////////////////////////////////////////////////////////////////////
RenderPlane::RenderPlane(const char* imagename){
	//Init(imagename);
}
////////////////////////////////////////////////////////////////////////////////
RenderPlane::RenderPlane(GameID materialID){
	/*
	GameID meshID = RendererObjectFactory::GetUnitPlaneMesh();

	mMeshes.push_back(meshID);
	mMaterials.push_back(materialID);

	Material& mat = RendererObjectFactory::GetMaterial(materialID);
	//mat.mZWrite = !mDisableZWrite;

	mGame.GetRenderer().AddRenderObject(mID);
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,materialID);
	*/

}
////////////////////////////////////////////////////////////////////////////////
RenderPlane::RenderPlane(PersistentData& storageData):RenderObject(storageData)
{

}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::PreSetup(PersistentData& storageData)
{
	std::string imgName = storageData.GetProperty("ImageName");
	mDelayLoadingTexture = storageData.GetPropertyB("DelayTextureLoad");
	mTextureID = 0;
	mTexturePath = imgName;
	//Init(imgName);
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::SetupMeshes(PersistentData& storageData)
{
	
	GameID meshID = RendererObjectFactory::GetUnitPlaneMesh();
	//if(mTexturePath.empty())
	//{
		char name[256];
		sprintf(name,"RenderPlane_Clone_%d",mID);
		meshID = RendererObjectFactory::CloneMesh(meshID,name);
	//}
	mMeshes.push_back(meshID);
	mGame.GetRenderer().Retain(mID,meshID);

	std::vector<IvVector2> uvVec;
	PersistentData* uvs = storageData.GetNodeFromPath("UVs");
	if(uvs != NULL)
	{
		std::list<PersistentData*>& uvList = uvs->GetChildren();
		//uvList.reverse();
		std::list<PersistentData*>::iterator it = uvList.begin();
		while(it != uvList.end())
		{
			IvVector2 uv;
			float u = (*it)->GetPropertyF("x");
			float v = (*it)->GetPropertyF("y");
			uv.Set(u,v);
			uvVec.push_back(uv);
			++it;
		}
		if(!uvVec.empty())
			SetUVs(uvVec);
	}
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::SetupMaterialsAndTextures(PersistentData& storageData)
{
	GameID matID = 0;
	mTextureID = 0;
	if(!mTexturePath.empty())// && !mDelayLoadingTexture)
	{
		//matID = RendererObjectFactory::CreateMaterial(mTexturePath.c_str());
		mTextureID = RendererObjectFactory::CreateTexture(mTexturePath.c_str(),mTexturePath.c_str(),mDelayLoadingTexture);
	}
	char matName[256];
	sprintf(matName,"RenderPlaneMaterial_%d%s",mID,mTexturePath.c_str());
	matID = RendererObjectFactory::CreateMaterial(matName);
	
	
	mMaterials.push_back(matID);
	Material& mat = RendererObjectFactory::GetMaterial(matID);

	
	if(mTextureID != 0)
	{
		mat.AddTexture(mTextureID);
		mGame.GetRenderer().Retain(mID,mTextureID);
		
	}

	PersistentData* materialData = storageData.GetNodeFromPath("Material");
	if(materialData != NULL)
		mat.Configure(*materialData);
	
	mGame.GetRenderer().Retain(mID,matID);
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::PostSetup(PersistentData& storageData)
{
	if(mTextureID == 0 && !mDelayLoadingTexture)
	{
		SetColor(mColor);
	}
	/*
	GameID meshID = RendererObjectFactory::GetUnitPlaneMesh();
	
	GameID matID = 0;
	mTextureID = 0;
	if(!imagename.empty() && !mDelayLoadingTexture)
	{
		matID = RendererObjectFactory::CreateMaterial(mTexturePath.c_str());
		mTextureID = RendererObjectFactory::CreateTexture(mTexturePath.c_str(),mTexturePath.c_str());
	}else{
		char matName[256];
		sprintf(matName,"RenderPlaneMaterial_%d%s",mID,imagename.c_str());
		matID = RendererObjectFactory::CreateMaterial(matName);
	}
	
	
	mMaterials.push_back(matID);

	Material& mat = RendererObjectFactory::GetMaterial(matID);
	//mat.mZWrite = !mDisableZWrite;
	
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,matID);
	if(mTextureID != 0)
	{
		mat.AddTexture(mTextureID, true);
		mGame.GetRenderer().Retain(mID,mTextureID);
		
	}
	else if(mTexturePath.empty())
	{
		char name[256];
		sprintf(name,"RenderPlane_Clone_Color_r=%3.2f,g=%3.2f,b=%3.2f,a=%3.2f",mColor.r,mColor.g,mColor.b,mColor.a);
		
		meshID = RendererObjectFactory::CloneMesh(meshID,name);
		mMeshes.push_back(meshID);
		SetColor(mColor);
	}
	if(mMeshes.empty())
		mMeshes.push_back(meshID);

	
	*/
	
}

////////////////////////////////////////////////////////////////////////////////
void RenderPlane::UnloadTexture()
{
	if(mTextureID == 0)
		return;
	/*
	GameID matID = mMaterials[0];
	Material& mat = RendererObjectFactory::GetMaterial(matID);
	mat.ClearTextures();
	mGame.GetRenderer().Release(mID,mTextureID);
	//Texture& t = RendererObjectFactory::GetTexture(mTextureID);
	mTextureID = 0;
	*/
	Texture& t = RendererObjectFactory::GetTexture(mTextureID);
	t.UnloadTextureData();
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::LoadTexture()
{
	if(mTextureID == 0)
	{
		return;
	}
	/*
	if(!mTexturePath.empty())
	{
		mTextureID = RendererObjectFactory::CreateTexture(mTexturePath.c_str(),mTexturePath.c_str());
		GameID matID = mMaterials[0];
		Material& mat = RendererObjectFactory::GetMaterial(matID);
	
		mat.AddTexture(mTextureID);
		mGame.GetRenderer().Retain(mID,mTextureID);
	}	
	*/
	Texture& t = RendererObjectFactory::GetTexture(mTextureID);
	t.LoadTextureData();
}
////////////////////////////////////////////////////////////////////////////////
RenderPlane::~RenderPlane(){
	mGame.GetRenderer().RemoveRenderObject(mID);
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
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("RenderPlane");
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);

}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::StartAsSequenceItem(PersistentData* storageData)
{		
	if(!mVisible) 
		mVisible = true;
	
	if(!mActive)
		mActive = true;
		
	//if(mTextureID != 0)
	//	LoadTexture();
	
	GameMessage msg;
	msg.mEventName = "FINISHED";
	msg.mSender = mID;
	SendOutMessage(msg);
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::StopAsSequenceItem()
{
	//mActive = false;
	
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderPlane::SequenceOver(bool deleteObject)
{
	
	// UnloadTexture();
	//mVisible = false;
	//mActive = false;
	//if(deleteObject)
	//	mGame.DeleteGameObject(mID);
	 
}
////////////////////////////////////////////////////////////////////////////////