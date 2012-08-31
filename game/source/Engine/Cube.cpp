#include "Cube.h"
#include "TextureFont.h"
#include "Game.h"
#include "Platform.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "Scene.h"
#include "Perlin.h"
#include "GameLogic.h"
////////////////////////////////////////////////////////////////////////////////
Cube::Cube()
{
	Init();
}
////////////////////////////////////////////////////////////////////////////////
Cube::Cube(PersistentData& storageData):RenderObject(storageData)
{
	Init();
	
	//ColliderID cID = mGame.GetPhysicsManager().ConstructBoxCollider(IvVector3(10.0f,10.0f,10.0f));
	//mPhysicsProxy = new PhysicsProxy((SceneObject&)*this,cID,true, 1.0f);
}
////////////////////////////////////////////////////////////////////////////////
Cube::~Cube()
{
	
}
////////////////////////////////////////////////////////////////////////////////
void Cube::Init()
{
	std::string meshName;
	char buff[64];
	GameID signMeshID = 0;
	GameID matID = 0;
	GameID texID = 0;
	
	
	//Now create the main part of the sign
	meshName = "Cube";
	sprintf(buff,"%03d",mID);
	meshName += buff;
	signMeshID = RendererObjectFactory::CreateUnitCubeMesh();
	signMeshID = RendererObjectFactory::CloneMesh(signMeshID,meshName);
	mMeshes.push_back(signMeshID);//has to be added to this list  - in order to be released;
	
	Mesh& m = RendererObjectFactory::GetMesh(signMeshID);
	for(unsigned int i = 0; i < m.mVertexCount; ++i)
	{
		m.mVertices[i].x *= 100.0f;
		m.mVertices[i].y *= 100.0f;
		m.mVertices[i].z *= 100.0f;
	}
	
	//FLIP the texture coords on the back so i can fake the rotation of the reveal
	m.mUV[4].Set(1,1);
	m.mUV[5].Set(0,1);
	m.mUV[6].Set(0,0);
	m.mUV[7].Set(1,0);
	
	//Darken the sides and bottom by referencing part of the texture
	for(int ii = 8; ii <= 23; ii++)
		m.mColors[ii].Set(0.5,0.5f,0.5f,1);
	//top face - x/z plane
	m.mUV[8].Set(0,1);
	m.mUV[9].Set(0,1);
	m.mUV[10].Set(0,0);
	m.mUV[11].Set(0,0);
	
	
	
	//bottom face x/z plane - reorder indices	
	m.mUV[12].Set(0,1);
	m.mUV[13].Set(0,1);
	m.mUV[14].Set(0,0);
	m.mUV[15].Set(0,0);
	
	
	//left face y/z plane	
	m.mUV[16].Set(0,1);
	m.mUV[17].Set(0,1);
	m.mUV[18].Set(0,0);
	m.mUV[19].Set(0,0);
	
	
	//right face y/z	
	m.mUV[20].Set(0,1);
	m.mUV[21].Set(0,1);
	m.mUV[22].Set(0,0);
	m.mUV[23].Set(0,0);
	
	
	
	char matName[256];
	std::string signMainImageName = "sign_plaque.png";
	sprintf(matName, "%s%03d",signMainImageName.c_str(),mID);
	matID = RendererObjectFactory::CreateMaterial(matName);
	texID = RendererObjectFactory::CreateTexture(signMainImageName.c_str(),signMainImageName.c_str());
	Material& matMain = RendererObjectFactory::GetMaterial(matID);
	matMain.SetMainTexture(texID);
	mMaterials.push_back(matID);
	
	mGame.GetRenderer().Retain(mID,texID);
	mGame.GetRenderer().Retain(mID,signMeshID);
	mGame.GetRenderer().Retain(mID,matID);
	
	mGame.GetRenderer().AddRenderObject(mID);//This is where it is specified that it is text
	
	
}
////////////////////////////////////////////////////////////////////////////////
void Cube::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("Cube");
	
}
////////////////////////////////////////////////////////////////////////////////
void Cube::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////