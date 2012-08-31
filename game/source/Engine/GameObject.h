/*
 *  GameObject.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/7/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H


#include "Globals.h"
#include "PersistentDataManager.h"
#include "MessageManager.h"
#include "TimeManager.h"
//#include "IvMathLib.h"
class Game;
class PersistentData;

class GameObject{

protected:
	GameID	mID;
	GameID	mOwner;
	Game&			mGame;
	std::string		mTag;
	bool			mActive;
	int				mLayerID;
	std::multimap<std::string, GameID>	mMessageListeners;
	std::list<std::pair<std::string,GameID> > mPendingListenerRemovals;
	
public:
	GameObject();
	GameObject(PersistentData& storageData);
	virtual ~GameObject();
	virtual void AddedToDeletionList(){};

	virtual void PreSetup(PersistentData& storageData){};
	virtual void SetupMeshes(PersistentData& storageData){};
	virtual void SetupMaterialsAndTextures(PersistentData& storageData){};
	virtual void PostSetup(PersistentData& storageData){};

	virtual GameID GetID(){ return mID;};
	void SetID(GameID id){mID = id;};
	GameID GetOwner(){ return mOwner;};
	std::string GetTag(){return mTag;};
	void SetTag(std::string& t);
	void SetTag(const char *s);	
	void SetOwner(GameID owner){ mOwner = owner; };
	bool IsActive(){return mActive;};
	virtual void SetActive(bool v){mActive = v;};
	int GetLayerID(){return mLayerID;};
	void SetLayerID(int id){mLayerID = id;};
	void Destroy();
	void DestroyImmediately();

	virtual void AddEventListener(std::string eventName, GameID listener);
	virtual void RemoveEventListener(std::string eventName,GameID listener);
	virtual void ActOnPendingListenerRemovals();
	virtual void SendOutMessage(GameMessage& msg);
	virtual void ProcessMessage(GameMessage& msg);
	virtual void StartAsSequenceItem(PersistentData* storageData = NULL){};
	virtual void StopAsSequenceItem(){};
	virtual void SequenceBegin(){};
	virtual void SequenceOver(bool deleteObject = true){};

	virtual void Serialize(PersistentData& storageData);
	virtual void DeSerialize(PersistentData& storageData);
	virtual void Update(){};
	virtual void LateUpdate();
	virtual void FixedUpdate(){};
	virtual void LevelLoaded(){};
	virtual void LevelStart(){};
	virtual void LevelEnd(){};

	virtual void MouseDown(float u, float v, int button){};
	virtual void MouseDragged(float u, float v,float lu, float lv, int button){};
	virtual void MouseUp(float u, float v, int button){};

	virtual void UpdateJoystick(float u, float v, float w){};
	
	virtual std::string GetTypeString()
	{
		return "GameObject";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "%d\t%s\t%s",GetID(),GetTypeString().c_str(),mTag.c_str());
		std::string s = buf;
		return s;
	}
	
private:
	//Remove Copy Constructor and assignment operator
	GameObject		&operator =  (const GameObject &m) { printf("GameObject = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	GameObject(const GameObject &m):mGame(m.mGame){printf("GameObject COPY CTOR SHOULDN'T BE GETTING CALLED");};
	
	
};


#endif //_GAME_OBJECT_H