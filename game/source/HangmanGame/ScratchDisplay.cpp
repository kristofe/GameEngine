#include "ScratchDisplay.h"
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
#include "RenderPlane.h"

////////////////////////////////////////////////////////////////////////////////
ScratchDisplay::ScratchDisplay(PersistentData& storageData):GameObject(storageData)
{
	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	if(storageData.GetPropertyB("RefsAreImmediate"))
	{
		mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	}
	std::list<PersistentData*> states = storageData.GetChildren();
	std::list<PersistentData*>::iterator it = states.begin();
	while(it != states.end())
	{
		PersistentData* node = *it;
		//unsigned int stateID = node->GetPropertyU("StateID");
		std::string tag = node->GetProperty("Tag");
		RenderPlane* rp = static_cast<RenderPlane*>(mGame.GetTaggedObject(tag));
		mRenderPlanes.push_back(rp);
		
		
		++it;
	}
	mCurrentRenderPlane = NULL;
	mCurrentStateID = 99;
	SetState(0);
}
////////////////////////////////////////////////////////////////////////////////
ScratchDisplay::~ScratchDisplay()
{

}
////////////////////////////////////////////////////////////////////////////////
void ScratchDisplay::LevelLoaded()
{
	//mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	
}
////////////////////////////////////////////////////////////////////////////////
void ScratchDisplay::SetState(unsigned int stateID)
{
	if(mCurrentStateID == stateID)
		return;
	mCurrentStateID = stateID;
	if(mCurrentRenderPlane != NULL)
	{
		//mCurrentRenderPlane->UnloadTexture();
		mCurrentRenderPlane->SetActive(false);
		mCurrentRenderPlane->SetVisible(false);
	}
	if(stateID < mRenderPlanes.size())
	{
		RenderPlane* rp = mRenderPlanes[stateID];
		mCurrentRenderPlane = rp;
		//rp->LoadTexture();
		rp->SetActive(true);
		rp->SetVisible(true);
		
	}
}
////////////////////////////////////////////////////////////////////////////////
void ScratchDisplay::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("ScratchDisplay");
	
}
////////////////////////////////////////////////////////////////////////////////
void ScratchDisplay::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
std::string ScratchDisplay::DebugOutput()
{
	char buf[256];
	unsigned int currID = 0;
	if(mCurrentRenderPlane != NULL)
	{
		currID = (unsigned int)mCurrentRenderPlane->GetID();
	}
	sprintf(buf, "\tRenderPlanesCount:%d\tCurrentRenderPlaneID:%d",(int)mRenderPlanes.size(),currID);
	std::string superInfo = GameObject::DebugOutput();
	std::string s = superInfo + buf;
	return s;
}
////////////////////////////////////////////////////////////////////////////////
