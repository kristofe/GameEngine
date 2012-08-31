#include "Game.h"
#include "Platform.h"
#include "FluidGrid.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "Mesh.h"
#include "Material.h"

////////////////////////////////////////////////////////////////////////////////
FluidGrid::FluidGrid(int nx, int ny, float width, float height,std::string imagename)
{
	
	Init(nx,ny,width,height,imagename);
}
////////////////////////////////////////////////////////////////////////////////
void FluidGrid::Init(int nx, int ny, float width, float height, std::string imagename)
{
	_NX = nx;
	_NY = ny;
	_gridCellsCount = _NX*_NY;
	_gridWidth = width;
	_gridHeight = height;
	_paused = true;

	char mesh_name[64];
	char mat_name[64];
	char tex_name[64];
	sprintf(mesh_name,"FluidGrid%3d_Mesh",mID);
	sprintf(mat_name,"FluidGrid%3d_Material",mID);
	sprintf(tex_name,"FluidGrid%3d_Texture",mID);
	GameID meshID = RendererObjectFactory::CreateGridMesh(mesh_name,nx,ny,width, height);
	GameID matID = RendererObjectFactory::CreateMaterial(mat_name);
	GameID texID = RendererObjectFactory::CreateTexture(tex_name,imagename.c_str());

	mMeshes.push_back(meshID);
	mMaterials.push_back(matID);

	Mesh& mesh = RendererObjectFactory::GetMesh(meshID);
	//Create Local Copy of mesh data for quick access and for bulk update calls to mesh
	mVertices = new IvVector3[mesh.mVertexCount];
	mColors = new IvColor[mesh.mColorCount];
	mUV = new IvVector2[mesh.mUVCount];
	mUV2 = new IvVector2[mesh.mUV2Count];
	memcpy(mVertices,mesh.mVertices,sizeof(IvVector3) * mesh.mVertexCount);
	memcpy(mColors,mesh.mColors,sizeof(IvColor) * mesh.mColorCount);
	memcpy(mUV,mesh.mUV,sizeof(IvVector2) * mesh.mUVCount);
	memcpy(mUV2,mesh.mUV2,sizeof(IvVector2) * mesh.mUV2Count);

	Material& mat = RendererObjectFactory::GetMaterial(matID);
	mat.AddTexture(texID);
	//mat.mZWrite = !mDisableZWrite;

	mGame.GetRenderer().AddRenderObject(mID);
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,matID);
	mGame.GetRenderer().Retain(mID,texID);
}
////////////////////////////////////////////////////////////////////////////////
FluidGrid::FluidGrid(PersistentData& storageData):RenderObject(storageData)
{
	std::string imagename = storageData.GetProperty("ImageName");
	int nx = storageData.GetPropertyI("nx");
	int ny = storageData.GetPropertyI("ny");
	float width = storageData.GetPropertyF("width");
	float height = storageData.GetPropertyF("height");

	Init(nx,ny,width,height,imagename.c_str());
	
}
////////////////////////////////////////////////////////////////////////////////
FluidGrid::~FluidGrid(){
	
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

	SAFE_DELETE_ARRAY(mVertices);
	SAFE_DELETE_ARRAY(mColors);
	SAFE_DELETE_ARRAY(mUV);
	SAFE_DELETE_ARRAY(mUV2);
	
}

////////////////////////////////////////////////////////////////////////////////
void FluidGrid::adjustUVs(float * vals)
{
	for( int y=0 ; y < _NY; y++ )
	{
		for( int x=0 ; x < _NX; x++ )
		{
			int idx = IX(x,y);
			float w = vals[idx];//Assumes that color is between -1 and 1;
			mUV[idx].x = mUV2[idx].x + w*0.08;
			mUV[idx].y = mUV2[idx].y + w*0.08;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void FluidGrid::setUVs(float * vals)
{
	//float v;
	for(int i = 0; i < _gridCellsCount-1; i++){
		float v = vals[i];
		mUV[i].Set(v,v);
	}
}
////////////////////////////////////////////////////////////////////////////////
void FluidGrid::setUVs(float * uVals, float * vVals)
{
	int i = 0;
	for(i = 0; i < _gridCellsCount-1; i++){
		mUV[i].x = uVals[i];
	}
	i = 0;
	for(i = 0; i < _gridCellsCount-1; i++){
		mUV[i].y = vVals[i];
	}
}

////////////////////////////////////////////////////////////////////////////////
void FluidGrid::adjustColorsOfVertices(IvColor * vals)
{
	for( int y=0 ; y < _NY; y++ )
	{
		for( int x=0 ; x < _NX; x++ )
		{
			int idx = IX(x,y);
			mColors[idx] = vals[idx]; 

		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void FluidGrid::adjustColorsOfVertices2(float * vals)
{
	for( int y=0 ; y < _NY; y++ )
	{
		for( int x=0 ; x < _NX; x++ )
		{
			int idx = IX(x,y);
			float color = vals[idx];//Assumes that color is between 0 and 1;
			mColors[idx].Set(color,color,color,1.0f);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void FluidGrid::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("FluidGrid");
	storageData.SetProperty("nx",_NX);
	storageData.SetProperty("ny",_NY);
	storageData.SetProperty("width",_gridWidth);
	storageData.SetProperty("height",_gridHeight);
	
}
////////////////////////////////////////////////////////////////////////////////
void FluidGrid::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);	
	_NX = storageData.GetPropertyI("nx");
	_NY = storageData.GetPropertyI("ny");
	_gridCellsCount = _NX*_NY;
	_gridWidth = storageData.GetPropertyF("width");
	_gridHeight = storageData.GetPropertyF("height");
	
	GameID meshID = mMeshes[0];
	Mesh& mesh = RendererObjectFactory::GetMesh(meshID);
	mVertices = new IvVector3[mesh.mVertexCount];
	mColors = new IvColor[mesh.mColorCount];
	mUV = new IvVector2[mesh.mUVCount];
	mUV2 = new IvVector2[mesh.mUV2Count];
	memcpy(mVertices,mesh.mVertices,sizeof(IvVector3) * mesh.mVertexCount);
	memcpy(mColors,mesh.mColors,sizeof(IvColor) * mesh.mColorCount);
	memcpy(mUV,mesh.mUV,sizeof(IvVector2) * mesh.mUVCount);
	memcpy(mUV2,mesh.mUV2,sizeof(IvVector2) * mesh.mUV2Count);
	
}
////////////////////////////////////////////////////////////////////////////////