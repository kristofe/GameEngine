#include "Game.h"
#include "Sequence.h"

////////////////////////////////////////////////////////////////////////////////
Sequence::Sequence()
{
	Init();
}
////////////////////////////////////////////////////////////////////////////////
Sequence::Sequence(PersistentData& storageData):GameObject(storageData)
{
	std::list<PersistentData*> nodes = storageData.GetChildren();
	//storageData.GetNodesFromPath("SequenceItem",nodes);
	std::list<PersistentData*>::iterator it = nodes.begin();
	while(it != nodes.end())
	{
		PersistentData* node =  *it;
		SequenceItemMetaData* md = new SequenceItemMetaData();
		md->mGameObjectTag = "";
		md->mDeactivate = false;
		md->mObjectType = node->GetName();
		mSequenceItems.push_back(md);
		md->mIsLinked = false;
		md->mStorageData = node;

		if(md->mObjectType == "TaggedObjectLink")
		{
			md->mGameObjectTag = node->GetProperty("Tag");
			md->mDeactivate = node->GetPropertyB("Deactivate");
			md->mIsLinked = true;
			PersistentData* data = node->GetNodeFromPath("Parameters");
			md->mStorageData = data;
		}
		++it;
	}
	
	Init();
 
	std::string mof = storageData.GetProperty("MessageOnFinish");
	if(!mof.empty())
		mMessageOnFinish = mof;

	if(mActive)
		StartAsSequenceItem();
}
////////////////////////////////////////////////////////////////////////////////
Sequence::~Sequence()
{
	mCurrentItemIndex = 0;
	mCurrentItem = NULL;
	mCurrentGameObject = NULL;
	for(int i = 0; i < (int)mSequenceItems.size(); ++i)
	{
		SequenceItemMetaData* item = mSequenceItems[i];
		SAFE_DELETE(item);
		mSequenceItems[i] = NULL;
	}
	mSequenceItems.clear();

}
////////////////////////////////////////////////////////////////////////////////
void Sequence::Init()
{
	mDone = false;
	mPendingIndex = -1;
	mCurrentItemIndex = 0;
	mCurrentItem = NULL;
	mCurrentGameObject = NULL;
	mMessageOnFinish = "SEQUENCE_FINISHED";
}
////////////////////////////////////////////////////////////////////////////////
void Sequence::ProcessMessage(GameMessage& msg)
{
	//if(mDone)
	//	return;
	
	if(msg.mEventName == "FINISHED")
	{
		if(mCurrentGameObject != NULL)
		{
			mCurrentGameObject->RemoveEventListener("FINISHED",mID);
			mCurrentGameObject->StopAsSequenceItem();
			

		}
		//mPendingIndex = mCurrentItemIndex+1;
		StartItem(mCurrentItemIndex+1);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Sequence::StartAsSequenceItem(PersistentData* storageData)
{
	mCurrentItem = NULL;
	if(mSequenceItems.empty())
	{
		return;
	}
	
	for(int i = 0; i < (int)mSequenceItems.size(); ++i)
	{
		SequenceItemMetaData* item = mSequenceItems[i];
		if(item->mIsLinked == true)
		{
			GameObject* go = mGame.GetTaggedObject(item->mGameObjectTag);
			if(go != NULL)
				go->SequenceBegin();
		}
	}
	StartItem(0);
	//mPendingIndex = 0;

}
////////////////////////////////////////////////////////////////////////////////
void Sequence::StartItem(int index)
{
	//mPendingIndex = -1;
	if(mCurrentItem != NULL)
	{
		if(mCurrentItem->mIsLinked == false)
		{
			mGame.DeleteGameObject(mCurrentGameObject->GetID());
			mCurrentGameObject = NULL;
		}
	}

	mCurrentItemIndex = index;
	if(mCurrentItemIndex >= (int)mSequenceItems.size())
	{
		for(int i = 0; i < (int)mSequenceItems.size(); ++i)
		{
			SequenceItemMetaData* item = mSequenceItems[i];
			GameObject* go = mGame.GetTaggedObject(item->mGameObjectTag);
			if(go != NULL)
				go->SequenceOver();
		}
		mCurrentItem = NULL;
		GameMessage msg;
		msg.mEventName = mMessageOnFinish;
		msg.mSender = mID;
		mDone = true;
		SendOutMessage(msg);
		
		
		return;
	}
	else
	{
		mCurrentItem = mSequenceItems[mCurrentItemIndex];
		if(mCurrentItem->mIsLinked)
		{
			mCurrentGameObject = mGame.GetTaggedObject(mCurrentItem->mGameObjectTag);
		}
		else
		{
			mCurrentGameObject = mGame.CreateGameObject(*(mCurrentItem->mStorageData));
		}
		mCurrentGameObject->AddEventListener("FINISHED",mID);
		mCurrentGameObject->StartAsSequenceItem(mCurrentItem->mStorageData);
	}
	

}
////////////////////////////////////////////////////////////////////////////////
void Sequence::Serialize(PersistentData& storageData)
{

}
////////////////////////////////////////////////////////////////////////////////
void Sequence::DeSerialize(PersistentData& storageData)
{

}
////////////////////////////////////////////////////////////////////////////////
void Sequence::Update()
{
	//if(mPendingIndex > -1)
	//{
	//	StartItem(mPendingIndex);
	//}
}
////////////////////////////////////////////////////////////////////////////////