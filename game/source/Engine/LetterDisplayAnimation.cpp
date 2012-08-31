#include "LetterDisplayAnimation.h"
#include "Game.h"
#include "Platform.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "MetaballManager2D.h"
#include "Scene.h"
#include "Perlin.h"
////////////////////////////////////////////////////////////////////////////////
LetterDisplayAnimation::LetterDisplayAnimation(TextureFont* font, MetaballGrid* metaballGrid,MetaballManager2D* metaballMgr2D, char c,IvVector3& start, IvVector3& end, unsigned int duration):LetterDisplay(font)
{
	mMetaballGrid = metaballGrid;
	mMetaballGrid->AddObject(mID,mID,start);
	mMetaballMgr2D = metaballMgr2D;
	
	Init();
	SetChar(c);
	mStartPos = start;
	mEndPos = end;
	mDuration = duration;

}
////////////////////////////////////////////////////////////////////////////////
LetterDisplayAnimation::~LetterDisplayAnimation()
{
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::Init()
{
	//mSelectedLetter = NULL;
	std::string meshName;
	meshName = "LetterDisplayAnimation";
	char buff[64];
	sprintf(buff,"%03d",mID);
	meshName += buff;
	GameID meshID = RendererObjectFactory::CreateMesh(meshName.c_str());
	mMeshes.push_back(meshID);//has to be added to this list  - in order to be released;
	mMaterials.push_back(mFont->materialID);//has to be added to this list  - in order to be released;
	
	mGame.GetRenderer().AddRenderObject(mID);//This is where it is specified that it is text
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,mFont->materialID);
	mGame.GetRenderer().Retain(mID,mFont->textureID);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::Serialize(PersistentData& storageData)
{
	LetterDisplay::Serialize(storageData);
	storageData.SetName("LetterDisplayAnimation");
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::DeSerialize(PersistentData& storageData)
{
	LetterDisplay::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::Update()
{
	if(mActive)
		Animate();

	if(mMetaballGrid != NULL)
	{
		mMetaballGrid->EffectField(mPosition.x, mPosition.y, LETTER_METABALL_RADIUS);
	}
	
	if(mMetaballMgr2D != NULL)
	{
		mMetaballMgr2D->SetMetaballPosition(mPosition, LETTER_METABALL_RADIUS_FOR_SPRITE);
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::Animate()
{
	unsigned int elapsed = GetTicks() - mStartTime;
	float t = elapsed/(float)mDuration;
	if(t > 1.0f)
	{
			GameMessage msg;
			msg.mEventName = "FINISHED";
			msg.mSender = mID;
			SendOutMessage(msg);
			return;
	}
	IvVector3 pos = Lerp(t, mStartPos, mEndPos);
	
	//Add noise to movement
	IvVector3 normDir = mEndPos - mStartPos;
	normDir.z = 0;
	normDir.Normalize();
	float x = normDir.x;
	float y = normDir.y;
	normDir.x = -y;
	normDir.y = x;
	
	float noise = Perlin::Noise(pos.x*0.01f,pos.y*0.01f);
	
	//ramp noise based on proximity to endpoints
	float s = 1.0f;
	if(t <= 0.1f)
		s = Perlin::lerp(t*10.0f,0.0f,1.0f);
	else if( t >= 0.9f)
		s = Perlin::lerp((t-0.9f)*10.0f,1.0f,0.0f);
	normDir *= noise*5.0f * s;
	
	pos += normDir;
	
	IvVector3 intPos;
	intPos.x = (int)pos.x;
	intPos.y = (int)pos.y;
	intPos.z = mStartPos.z;

	SetPosition(intPos);
	IvVector3 vel(0,201,0);
	mMetaballGrid->ObjectMoved(mID, intPos,vel);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::StartAsSequenceItem(PersistentData* storageData)
{
	mActive = true;
	mVisible = true;
	mStartTime = GetTicks();
	SetPosition(mStartPos);
	//mGame.GetActiveScene()->AddGameObject(this);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplayAnimation::StopAsSequenceItem()
{
	mActive = false;
	//mGame.GetActiveScene()->RemoveGameObject(this);//Handled in Game::RemoveGameObject
}
////////////////////////////////////////////////////////////////////////////////