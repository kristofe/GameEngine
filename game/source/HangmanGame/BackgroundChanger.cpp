#include "BackgroundChanger.h"
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
BackgroundChanger::BackgroundChanger(PersistentData& storageData):GameObject(storageData)
{
	mBackgroundIndex = storageData.GetPropertyU("BackgroundIndex");

	
	
}
////////////////////////////////////////////////////////////////////////////////
BackgroundChanger::~BackgroundChanger()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void BackgroundChanger::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("BackgroundChanger");
	
}
////////////////////////////////////////////////////////////////////////////////
void BackgroundChanger::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void BackgroundChanger::Update()
{

	
}

////////////////////////////////////////////////////////////////////////////////
void BackgroundChanger::StartAsSequenceItem(PersistentData* storageData)
{		
	GameLogic::GetInstance().SetBackground(mBackgroundIndex);
	//GameLogic::GetInstance().GetLetterChooser().AdjustLayout();
	GameMessage msg;
	msg.mEventName = "FINISHED";
	msg.mSender = mID;
	SendOutMessage(msg);
	
}
////////////////////////////////////////////////////////////////////////////////
void BackgroundChanger::StopAsSequenceItem()
{
	mActive = false;
	
}

////////////////////////////////////////////////////////////////////////////////