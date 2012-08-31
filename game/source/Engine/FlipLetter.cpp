#include "FlipLetter.h"
#include "WordProgressDisplay.h"
#include "LetterChooser.h"
#include "Game.h"
#include "GameLogic.h"
#include "Platform.h"
#include "Globals.h"
#include "Scene.h"
////////////////////////////////////////////////////////////////////////////////
FlipLetter::FlipLetter(PersistentData& storageData):GameObject(storageData)
{

	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	if(storageData.GetPropertyB("RefsAreImmediate"))
	{
		mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	}
}
////////////////////////////////////////////////////////////////////////////////
FlipLetter::~FlipLetter()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void FlipLetter::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("FlipLetter");
	
}
////////////////////////////////////////////////////////////////////////////////
void FlipLetter::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}

////////////////////////////////////////////////////////////////////////////////
void FlipLetter::StartAsSequenceItem(PersistentData* storageData)
{		
	WordProgressDisplay* wpd = mGameLogic->GetWordProgressDisplay();
	wpd->LetterChosen(mGameLogic->GetLastChosenLetter());

	GameMessage msg;
	msg.mEventName = "FINISHED";
	msg.mSender = mID;
	SendOutMessage(msg);
	
}
////////////////////////////////////////////////////////////////////////////////
void FlipLetter::StopAsSequenceItem()
{
	mActive = false;

	
}
////////////////////////////////////////////////////////////////////////////////