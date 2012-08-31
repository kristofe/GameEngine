#include "DisableInput.h"
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
DisableInput::DisableInput(PersistentData& storageData):GameObject(storageData)
{
	mValue = storageData.GetPropertyB("value");
	mFinished = false;
	
}
////////////////////////////////////////////////////////////////////////////////
DisableInput::~DisableInput()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void DisableInput::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("DisableInput");
	
}
////////////////////////////////////////////////////////////////////////////////
void DisableInput::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void DisableInput::Update()
{
	
	//if(mFinished == true)
	//	StopAsSequenceItem();
}

////////////////////////////////////////////////////////////////////////////////
void DisableInput::StartAsSequenceItem(PersistentData* storageData)
{		
	GameLogic::GetInstance().SetAllowChoosing(!mValue);
	
	GameMessage msg;
	msg.mEventName = "FINISHED";
	msg.mSender = mID;
	mFinished = true;
	SendOutMessage(msg);
	
}
////////////////////////////////////////////////////////////////////////////////
void DisableInput::StopAsSequenceItem()
{
	mActive = false;

	
}

////////////////////////////////////////////////////////////////////////////////