/*
 *  GameObject.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/7/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "Game.h"
#include "Scene.h"
#include "GameObject.h"
#include "LayerManager.h"

GameObject::GameObject() : mGame(Game::GetInstance()), mOwner(0),mLayerID(0){
	mID = mGame.RegisterGameObject(this);
}

GameObject::~GameObject(){
	mOwner = 0;
	mID = 0;
	mTag = "DELETED";
	mActive = false;
}

GameObject::GameObject(PersistentData& storageData):mGame(Game::GetInstance()), mOwner(0),mLayerID(0)
{
	/*mID = */mGame.RegisterGameObject(this);
	std::string s = storageData.GetProperty("Tag");
	SetTag(s);	
	mActive = storageData.GetPropertyB("Active");
	std::string layer = storageData.GetProperty("Layer");
	if(layer != "")
	{
		mLayerID = LayerManager::GetInstance().GetLayerID(layer);
	}
}

void GameObject::SetTag(std::string& t)
{
	if(t.empty()) return;
	mTag = t; 
	mGame.AddTaggedObject(this);
}

void GameObject::SetTag(const char *s)
{
	std::string t = s;
	SetTag(t);
}

void GameObject::Serialize(PersistentData& storageData)
{
	storageData.SetName("GameObject");
	
	char buff[128];
	sprintf(buff, "%d",mID);
	storageData.SetProperty("ID",buff);
	
	sprintf(buff, "%d",mOwner);
	storageData.SetProperty("Owner",buff);
	
	storageData.SetProperty("Tag",mTag.c_str());
	
}


void GameObject::DeSerialize(PersistentData& storageData)
{
	mID = static_cast<GameID>(storageData.GetPropertyU("ID"));
	mOwner = static_cast<GameID>(storageData.GetPropertyU("Owner"));
	std::string s = storageData.GetProperty("Tag");
	SetTag(s);
}
////////////////////////////////////////////////////////////////////////////////
void GameObject::AddEventListener(std::string eventName, GameID listener)
{
	mMessageListeners.insert(std::pair<std::string,GameID>(ToUpper(eventName),listener));
}
////////////////////////////////////////////////////////////////////////////////
void GameObject::RemoveEventListener(std::string eventName,GameID listener)
{
	mPendingListenerRemovals.push_back(std::pair<std::string, GameID>(eventName,listener));

}
////////////////////////////////////////////////////////////////////////////////
void GameObject::ActOnPendingListenerRemovals()
{
	std::string eventName;
	GameID listener;
	std::list<std::pair<std::string,GameID> >::iterator removals = mPendingListenerRemovals.begin();
	while(removals != mPendingListenerRemovals.end())
	{
		eventName = ToUpper(removals->first);
		listener = removals->second;

		std::list<GameID> objs;
		std::multimap<std::string,GameID>::iterator it = mMessageListeners.find(eventName);
		while(it != mMessageListeners.end())
		{
			if(it->second != listener)
				objs.push_back(it->second);
			++it;
		}

		mMessageListeners.erase(eventName);

		std::list<GameID>::iterator objIt = objs.begin();
		while(objIt != objs.end())
		{
			mMessageListeners.insert(std::pair<std::string,GameID>(eventName,*objIt));
			++objIt;
		}
		++removals;
	}
	mPendingListenerRemovals.clear();
	
}

////////////////////////////////////////////////////////////////////////////////
void GameObject::SendOutMessage(GameMessage& msg)
{
	if(msg.mBroadcast)
	{
		mGame.BroadcastMessage(msg);
		return;
	}

	std::multimap<std::string,GameID>::iterator it = mMessageListeners.find(ToUpper(msg.mEventName));
	while(it != mMessageListeners.end())
	{
		mGame.SendOutMessage(msg,it->second);
		++it;
	}

	std::string allEvents = "ALL";
	it = mMessageListeners.find(ToUpper(allEvents));
	while(it != mMessageListeners.end())
	{
		mGame.SendOutMessage(msg,it->second);
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////
void GameObject::ProcessMessage(GameMessage& msg)
{

}
////////////////////////////////////////////////////////////////////////////////
void GameObject::LateUpdate()
{
	if(!mPendingListenerRemovals.empty())
		ActOnPendingListenerRemovals();
}
////////////////////////////////////////////////////////////////////////////////
