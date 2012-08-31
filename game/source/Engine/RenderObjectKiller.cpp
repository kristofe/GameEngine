
#include "RenderObjectKiller.h"
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
RenderObjectKiller::RenderObjectKiller(PersistentData& storageData):GameObject(storageData)
{
	mTag = storageData.GetProperty("Tag");
	mDeactivateOnly = storageData.GetPropertyB("DeactivateOnly");
	
}
////////////////////////////////////////////////////////////////////////////////
RenderObjectKiller::~RenderObjectKiller()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void RenderObjectKiller::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("RenderObjectKiller");
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObjectKiller::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void RenderObjectKiller::Update()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void RenderObjectKiller::StartAsSequenceItem(PersistentData* storageData)
{		
	mGameObjectTag = storageData->GetProperty("GameObjectTag");
	mDeactivateOnly = storageData->GetPropertyB("DeactivateOnly");
	
	RenderObject* ro = static_cast<RenderObject*>(mGame.GetTaggedObject(mGameObjectTag));
	if(ro != NULL)
	{
		if(mDeactivateOnly)
		{
			ro->SetActive(false);
		}
		else
		{
			ro->SetActive(false);
			mGame.DeleteGameObject(ro->GetID());
		}
	}
	
	GameMessage msg;
	msg.mEventName = "FINISHED";
	msg.mSender = mID;

	SendOutMessage(msg);
	StopAsSequenceItem();
}
////////////////////////////////////////////////////////////////////////////////
void RenderObjectKiller::StopAsSequenceItem()
{
	mActive = false;
	
	
}

////////////////////////////////////////////////////////////////////////////////

