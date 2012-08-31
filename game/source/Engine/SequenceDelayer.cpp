#include "SequenceDelayer.h"
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
SequenceDelayer::SequenceDelayer(PersistentData& storageData):GameObject(storageData)
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
SequenceDelayer::~SequenceDelayer()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void SequenceDelayer::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("SequenceDelayer");
	
}
////////////////////////////////////////////////////////////////////////////////
void SequenceDelayer::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void SequenceDelayer::Update()
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
void SequenceDelayer::StartAsSequenceItem(PersistentData* storageData)
{		
	mStartTime = GetTicks();
	mActive = true;
	
}
////////////////////////////////////////////////////////////////////////////////
void SequenceDelayer::StopAsSequenceItem()
{
	mActive = false;
	
}
////////////////////////////////////////////////////////////////////////////////