#include "MetaballManager2D.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"

////////////////////////////////////////////////////////////////////////////////
MetaballManager2D::MetaballManager2D()
{

}
////////////////////////////////////////////////////////////////////////////////
MetaballManager2D::MetaballManager2D(PersistentData& storageData):RenderObject(storageData)
{
		
	std::string meshName;
	meshName = "MetaballManager2D";
	
	int poolSize = storageData.GetPropertyI("PoolSize");
	if(poolSize == 0)
		poolSize = 100;
	mTrianglePoolSize = poolSize;
	
	std::string mTexturePath = storageData.GetProperty("ImageName");
	
	
	char buff[64];
	sprintf(buff,"MetaballManager2D_MESH_%03d",mID);
	meshName += buff;
	GameID meshID = RendererObjectFactory::CreateMesh(meshName.c_str());
	mMeshes.push_back(meshID);//has to be added to this list  - in order to be released;
	
	GameID matID = 0;
	mTextureID = 0;
	if(!mTexturePath.empty())
	{
		//matID = RendererObjectFactory::CreateMaterial(mTexturePath.c_str());
		mTextureID = RendererObjectFactory::CreateTexture(mTexturePath.c_str(),mTexturePath.c_str());
	}
	char matName[256];
	sprintf(matName,"MetaballManager2DMaterial_%d%s",mID,mTexturePath.c_str());
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

	mGame.GetRenderer().AddRenderObject(mID);//This is where it is specified that it is text
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,matID);

	PreAllocateMesh(mTrianglePoolSize);
}

////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("MetaballManager2D");
	



}
////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);

	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::Update()
{
	if(mActive == false)
		return;
	
	//if(mDirtyMesh)
	//{
		UpdateMeshFromLocalData();
	//}
	
	
}


////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::SetMetaballPosition(const IvVector3& position, const float radius)
{
	for(int i = 0; i < 1; i++)
	{
		int startIDX = mVertices.size();
		float x1,y1,x2,y2,u1,u2,v1,v2;
		x1 = position.x - radius;
		x2 = position.x + radius;
		
		y1 = position.y - radius;
		y2 = position.y + radius;
		
		u1 = v2 = 0.0f;
		u2 = v1 = 1.0f;
		
		IvVector3 pos;
		pos.Set(x1,y1,0);
		mVertices.push_back(pos);
		pos.Set(x2,y1,0);
		mVertices.push_back(pos);
		pos.Set(x2,y2,0);
		
		mVertices.push_back(pos);
		pos.Set(x1,y2,0);
		mVertices.push_back(pos);
		
		IvColor c(1.0f,1.0f,1.0f,1.0f);
		mColors.push_back(c);
		mColors.push_back(c);
		mColors.push_back(c);
		mColors.push_back(c);
		
		IvVector2 uv;
		uv.Set(u1,v1);
		mUVs.push_back(uv);
		uv.Set(u2,v1);
		mUVs.push_back(uv);
		uv.Set(u2,v2);
		mUVs.push_back(uv);
		uv.Set(u1,v2);
		mUVs.push_back(uv);
		
		mIndices.push_back(startIDX + 0);
		mIndices.push_back(startIDX + 2);
		mIndices.push_back(startIDX + 3);
		
		mIndices.push_back(startIDX + 0);
		mIndices.push_back(startIDX + 1);
		mIndices.push_back(startIDX + 2);
	}
	mDirtyMesh = true;
}
////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::PreAllocateMesh(int triangleCount)
{
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	m.Clear();
	m.mVertexCount = triangleCount*3;
	//m.mNormalCount = mNormal;
	m.mColorCount = triangleCount*3;
	m.mUVCount = triangleCount*3;
	m.mTriangleCount = triangleCount;
	
	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];
	
	m.mTriangles = new unsigned short[triangleCount*3];
	memset(m.mTriangles,0,sizeof(unsigned short) * m.mTriangleCount);
	memset(m.mColors,0,sizeof(IvColor) * m.mColorCount);
	m.mTriangleCount = 0;//So that the mesh doesn't draw anything to start when there is garbage data... prevents a freeze crash

	m.Unlock();
	
	
}
////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::ConstructMeshFromLocalData()
{
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	m.Clear();
	m.mVertexCount = mVertices.size();
	//m.mNormalCount = mNormal;
	m.mColorCount = mColors.size();
	m.mUVCount = mUVs.size();
	m.mTriangleCount = mIndices.size()/3;
	
	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];
	
	m.mTriangles = new unsigned short[mIndices.size()];
	
	for(int i = 0; i < (int)mVertices.size(); i++)
	{
		m.mVertices[i] = mVertices[i];
	}
	
	
	for(int i = 0; i < (int)mColors.size(); i++)
	{
		m.mColors[i] = mColors[i];
	}
	
	for(int i = 0; i < (int)mUVs.size(); i++)
	{
		m.mUV[i] = mUVs[i];
	}	
	
	for(int i = 0; i < (int)mIndices.size(); i++)
	{
		m.mTriangles[i] = mIndices[i];
	}
	m.Unlock();
	
	
}

////////////////////////////////////////////////////////////////////////////////
void MetaballManager2D::UpdateMeshFromLocalData()
{
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	for(int i = 0; i < (int)mVertices.size(); i++)
	{
		m.mVertices[i] = mVertices[i];
	}
	
	
	for(int i = 0; i < (int)mColors.size(); i++)
	{
		m.mColors[i] = mColors[i];
	}
	
	for(int i = 0; i < (int)mUVs.size(); i++)
	{
		m.mUV[i] = mUVs[i];
	}	
	
	m.mTriangleCount = mIndices.size()/3;
	memset(m.mTriangles,0,sizeof(unsigned short) * m.mTriangleCount);
	for(int i = 0; i < (int)m.mTriangleCount*3; i++)
	{
		m.mTriangles[i] = mIndices[i];
	}
	
	
	
	m.Unlock();
	mDirtyMesh = false;
	mVertices.clear();
	mColors.clear();
	mUVs.clear();
	mIndices.clear();
	
}
////////////////////////////////////////////////////////////////////////////////