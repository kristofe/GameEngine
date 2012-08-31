#include "HangmanStateDisplay.h"
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
HangmanStateDisplay::HangmanStateDisplay(PersistentData& storageData):GameObject(storageData)
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
		unsigned int stateID = node->GetPropertyU("StateID");
		std::string tag = node->GetProperty("Tag");
		RenderPlane* rp = static_cast<RenderPlane*>(mGame.GetTaggedObject(tag));
		mRenderPlanes.push_back(rp);
		
		BlockerData* bd = new BlockerData();
		mBlockedAreasPerState.push_back(bd);
		bd->state = (int)stateID;
		
		std::list<PersistentData*> blockers;
		node->GetChildren("BlockedDropletArea", blockers);
		std::list<PersistentData*>::iterator blockerIt = blockers.begin();
		while(blockerIt != blockers.end())
		{
			IvVector3 min, max;
			PersistentData* blocker = *blockerIt;
			if(blocker->GetNodeFromPath("Min") != NULL)
			{
				min = blocker->GetVector3Child("Min");
			}
			if(blocker->GetNodeFromPath("Max") != NULL)
			{
				max = blocker->GetVector3Child("Max");
			}
			IvAABB aabb(min,max);
			bd->blockers.push_back(aabb);

			++blockerIt;
		}
		
		
		++it;
	}
	mCurrentRenderPlane = NULL;
	mCurrentStateID = 99;
	SetState(0);
}
////////////////////////////////////////////////////////////////////////////////
HangmanStateDisplay::~HangmanStateDisplay()
{
	for(unsigned int i = 0; i < mBlockedAreasPerState.size(); i++)
	{
		BlockerData* bd = mBlockedAreasPerState[i];
		bd->blockers.clear();
		SAFE_DELETE(bd);
	}
	mBlockedAreasPerState.clear();
}
////////////////////////////////////////////////////////////////////////////////
void HangmanStateDisplay::LevelLoaded()
{
	//mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));

}
////////////////////////////////////////////////////////////////////////////////
void HangmanStateDisplay::SetState(unsigned int stateID)
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
void HangmanStateDisplay::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("HangmanStateDisplay");
	
}
////////////////////////////////////////////////////////////////////////////////
void HangmanStateDisplay::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void HangmanStateDisplay::GetBlockedDropletAreas(std::vector<IvAABB>& aabbs, int stateID)
{
	BlockerData* bd = mBlockedAreasPerState[stateID];
	if(bd != NULL)
	{
		if(!bd->blockers.empty())
		{

			for(unsigned int i = 0; i < bd->blockers.size(); i++)
			{
				aabbs.push_back(bd->blockers[i]);
			}
		}
			
	}
}
////////////////////////////////////////////////////////////////////////////////