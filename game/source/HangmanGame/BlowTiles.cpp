#include "BlowTiles.h"
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
////////////////////////////////////////////////////////////////////////////////
BlowTiles::BlowTiles(PersistentData& storageData):GameObject(storageData)
{
	//mGameLogicTag = storageData.GetProperty("GameLogicTag");
	//mMetaballGridTag = storageData.GetProperty("TargetMetaballGrid");
	//if(storageData.GetPropertyB("RefsAreImmediate"))
	//{
	//	mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	//}
	mDuration = storageData.GetPropertyU("Duration");
	
}
////////////////////////////////////////////////////////////////////////////////
BlowTiles::~BlowTiles()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void BlowTiles::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("BlowTiles");
	
}
////////////////////////////////////////////////////////////////////////////////
void BlowTiles::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void BlowTiles::Update()
{
	if(!mActive)
		return;
	
	unsigned int elapsed = GetTicks() - mStartTime;
	if(elapsed >= mDuration)
	{
		
		GameMessage msg;
		msg.mEventName = "FINISHED";
		msg.mSender = mID;
		SendOutMessage(msg);
	}
	
}

////////////////////////////////////////////////////////////////////////////////
void BlowTiles::StartAsSequenceItem(PersistentData* storageData)
{		
	mStartTime = GetTicks();
	mActive = true;
	//GameLogic::GetInstance().StartSoundEffect("WIND");
	GameLogic::GetInstance().BlowTiles();
	
}
////////////////////////////////////////////////////////////////////////////////
void BlowTiles::StopAsSequenceItem()
{
	mActive = false;
	//GameLogic::GetInstance().StopSoundEffect();
	
}
////////////////////////////////////////////////////////////////////////////////