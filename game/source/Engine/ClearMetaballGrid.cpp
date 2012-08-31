#include "ClearMetaballGrid.h"
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
ClearMetaballGrid::ClearMetaballGrid(PersistentData& storageData):GameObject(storageData)
{
	mFinished = false;
	
}
////////////////////////////////////////////////////////////////////////////////
ClearMetaballGrid::~ClearMetaballGrid()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void ClearMetaballGrid::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("ClearMetaballGrid");
	
}
////////////////////////////////////////////////////////////////////////////////
void ClearMetaballGrid::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void ClearMetaballGrid::Update()
{
	
	//if(mFinished == true)
	//	StopAsSequenceItem();
}

////////////////////////////////////////////////////////////////////////////////
void ClearMetaballGrid::StartAsSequenceItem(PersistentData* storageData)
{		
	GameLogic::GetInstance().ClearMetaballGrid();
	GameLogic::GetInstance().GetLetterChooser().Clear();//Hack!!! Should be its own sequence item... but it makes sense that if you are going to clear the metaball grid you are going to reset the letter chooser too.
	GameMessage msg;
	msg.mEventName = "FINISHED";
	msg.mSender = mID;
	mFinished = true;
	SendOutMessage(msg);
	
}
////////////////////////////////////////////////////////////////////////////////
void ClearMetaballGrid::StopAsSequenceItem()
{
	mActive = false;
	
	
}

////////////////////////////////////////////////////////////////////////////////