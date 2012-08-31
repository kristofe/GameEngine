#include "ScreenFader.h"
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
ScreenFader::ScreenFader(PersistentData& storageData):RenderObject(storageData)
{
	mDuration = storageData.GetPropertyU("Duration");
	mStartColor = storageData.GetColorChild("StartColor");
	mEndColor = storageData.GetColorChild("EndColor");
	//mZPosition = storageData.GetPropertyF("ZPosition");

	GameID meshID = RendererObjectFactory::GetUnitPlaneMesh();
	char name[512];
	sprintf(name,"ScreenFaderMesh_%d",mID);
	meshID = RendererObjectFactory::CloneMesh(meshID,name);
	
//	Mesh& m = RendererObjectFactory::GetMesh(meshID);
	sprintf(name,"ScreenFaderMaterial_%d",mID);
	GameID matID = RendererObjectFactory::CreateMaterial(name);
	Material& mat = RendererObjectFactory::GetMaterial(matID);
	mat.mAmbient.Set(0,0,0,0);
	mat.mDiffuse.Set(0,0,0,0);
	
	mMeshes.push_back(meshID);
	mMaterials.push_back(matID);

	//Material& mat = RendererObjectFactory::GetMaterial(matID);
	//mat.mZWrite = !mDisableZWrite;
	
	mGame.GetRenderer().AddRenderObject(mID);
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,matID);

	//Now position and scale the plane
	int h = mGame.GetRenderer().GetWindowHeight();
	int w = mGame.GetRenderer().GetWindowWidth();
	IvVector3 pos;
	GetLocalPosition(pos);
	ResetLocalTransform();
	Translate(pos);
	IvVector3 scale(w,h,1);
	Scale(scale);

	
}
////////////////////////////////////////////////////////////////////////////////
ScreenFader::~ScreenFader()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void ScreenFader::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("ScreenFader");
	
}
////////////////////////////////////////////////////////////////////////////////
void ScreenFader::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void ScreenFader::Update()
{
	if(!mActive)
		return;

	unsigned int elapsed = GetTicks() - mStartTime;
	float t = elapsed/(float)mDuration;
	mCurrentColor = Lerp(t,mStartColor,mEndColor);
	SetColor(mCurrentColor);

	if(elapsed >= mDuration)
	{
		
		GameMessage msg;
		msg.mEventName = "FINISHED";
		msg.mSender = mID;
		SendOutMessage(msg);
	}
	
}

////////////////////////////////////////////////////////////////////////////////
void ScreenFader::StartAsSequenceItem(PersistentData* storageData)
{		
	mStartTime = GetTicks();
	mActive = true;
	mCurrentColor = mStartColor;
	SetColor(mCurrentColor);
	
}
////////////////////////////////////////////////////////////////////////////////
void ScreenFader::StopAsSequenceItem()
{
	mActive = false;
	
}

////////////////////////////////////////////////////////////////////////////////