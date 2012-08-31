#include "Game.h"
#include "Platform.h"
#include "SimpleParticleSystem.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Perlin.h"
#include "DebugText.h"

////////////////////////////////////////////////////////////////////////////////
SimpleParticleSystem::SimpleParticleSystem(PersistentData& storageData):RenderObject(storageData)
{
    
	mPoolSize = storageData.GetPropertyI("PoolSize");
    mMinParticleSize = storageData.GetVector2Child("MinSize");
    mMaxParticleSize = storageData.GetVector2Child("MaxSize");
	mCurrentParticleSize = mMinParticleSize;
	mDebugTextTag = storageData.GetNodeFromPath("DebugText")->GetProperty("Tag");
    mParticles.reserve(mPoolSize);
    mParticlePositions.reserve(mPoolSize);
    
	char mesh_name[128];
	char mat_name[128];
	sprintf(mesh_name,"SimpleParticleSystem%3d_Mesh",mID);
	sprintf(mat_name,"SimpleParticleSystem%3d_Material",mID);

	GameID meshID = RendererObjectFactory::CreateQuadPoolMesh(mesh_name, mPoolSize, mMinParticleSize);
	GameID matID = RendererObjectFactory::CreateMaterial(mat_name);
	
    PersistentData* materialData = storageData.GetNodeFromPath("Material");
	if(materialData != NULL)
    {
        Material& mat = RendererObjectFactory::GetMaterial(matID);
        mat.Configure(*materialData);
    }
    
	mMeshes.push_back(meshID);
	mMaterials.push_back(matID);
    
    mGame.GetRenderer().AddRenderObject(mID);
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,matID);
	
	mLastUpdateTime = 0;
	mElapsedSecondsSinceLastUpdate = 0.0f;
    InitParticles();
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::LevelLoaded()
{
	UpdateDebugText();
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::UpdateDebugText()
{
    DebugText* dt = static_cast<DebugText*>(mGame.GetTaggedObject(mDebugTextTag));
    char msg[128];
	sprintf(msg,"%3d particles of size (%3.0f, %3.0f)",mPoolSize,mCurrentParticleSize.x,mCurrentParticleSize.y);
    dt->SetString(msg);
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::MouseDown(float u, float v, int button)
{
	if(button == 0)
	{
	if(mCurrentParticleSize.x == mMinParticleSize.x)
		mCurrentParticleSize = mMaxParticleSize;
	else 
		mCurrentParticleSize = mMinParticleSize;
	}
	else if(button == 1)
	{
		
	}
	UpdateDebugText();
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::InitParticles()
{
    for(int i = 0; i < mPoolSize; i++)
    {
        SimpleParticleSystem::SimpleParticle& p = mParticles[i];
        p.mParticlePositionID = i;
        p.mParticlePoolID = i;
		p.mVelocity.Set((float)rand()/RAND_MAX,(float)rand()/RAND_MAX);
        p.mVelocity.Normalize();
		p.mVelocity *= ((float)rand()/RAND_MAX)*20.0f;
    }
    srand(GetTicks());
    for(int i = 0; i < mPoolSize; i++)
    {
        IvVector3& p = mParticlePositions[i];
        float x = rand() % 320;
        float y = rand() % 480;
        float z = rand() % 20 + 10;
        p.Set(x,y,z);
        
    }
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::Update()
{
	if(mLastUpdateTime != 0)
	{
		mElapsedSecondsSinceLastUpdate = (GetTicks() - mLastUpdateTime) * 0.001f;
	}
    UpdateParticles();
    UpdateMesh();
	mLastUpdateTime = GetTicks();
    
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::UpdateParticles()
{
    for(int i = 0; i < mPoolSize; i++)
    {
		SimpleParticleSystem::SimpleParticle particle = mParticles[i];
        IvVector3& p = mParticlePositions[i];
        float noisex = Perlin::Noise(p.x*0.1f,p.y*0.1f,GetTicks()*0.001f*0.5f);
        float noisey = Perlin::Noise(p.x*-0.1f,p.y*-0.1f,GetTicks()*0.001f*0.5f);
        float x = p.x + particle.mVelocity.x * mElapsedSecondsSinceLastUpdate;//noisex*1.5f;
        float y = p.y + particle.mVelocity.y * mElapsedSecondsSinceLastUpdate;;//noisey*1.5f;
        float z = rand() % 20 + 10;
        if(x > 320 + mCurrentParticleSize.x*0.5f)
            x = -mCurrentParticleSize.x * 0.5f;
        if(x < -mCurrentParticleSize.x*0.5f)
            x = 320 - mCurrentParticleSize.x * 0.5f;
        if(y > 480 + mCurrentParticleSize.y*0.5f)
            y = -mCurrentParticleSize.y * 0.5f;
        if(y < -mCurrentParticleSize.y*0.5f)
            y = 480 - mCurrentParticleSize.y * 0.5f;
        
        p.Set(x,y,z);
    }

}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::UpdateMesh()
{   
    float sx = 0.5f * mCurrentParticleSize.x;
    float sy = 0.5f * mCurrentParticleSize.y;
    GameID meshID = mMeshes[0];
    Mesh& m = RendererObjectFactory::GetMesh(meshID);    
    int index = 0;
    for(int i = 0; i < mPoolSize; i++)
    {
        IvVector3& p = mParticlePositions[i];
        index = i * 4;
        m.mVertices[index + 0].Set(-sx + p.x,-sy + p.y, p.z);
        m.mVertices[index + 1].Set( sx + p.x,-sy + p.y, p.z);
        m.mVertices[index + 2].Set( sx + p.x, sy + p.y, p.z);
        m.mVertices[index + 3].Set(-sx + p.x, sy + p.y, p.z);
    }
}
////////////////////////////////////////////////////////////////////////////////
SimpleParticleSystem::~SimpleParticleSystem(){
    mParticles.clear();
    mParticlePositions.clear();
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("SimpleParticleSystem");
	
}
////////////////////////////////////////////////////////////////////////////////
void SimpleParticleSystem::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);
    
}
////////////////////////////////////////////////////////////////////////////////