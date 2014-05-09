/*
 *  Game.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 12/25/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */

#ifndef MAIN_SIM_H
#define MAIN_SIM_H

#include "SimBase.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "PhysicsManager.h"
#include <map>

class PersistentDataManager;
class Renderer;
class Scene;
class TimerManager;
//class MessageManager;

class Game : public SimBase{
public:
	//friend class MessageManager;

	static Game& GetInstance();
	Game();
	~Game();
	
	void UpdateAccelerometerAndOrientation(const IvVector3& accel, const IvVector3& orient);
	void Update();
	void LateUpdate();
	void FixedUpdate();
	//void Render();
	void MouseDown(float u, float v, int button);
	void MouseDragged(float u, float v,float lu,float lv, int button);
	void MouseUp(float u, float v, int button);

	void Start(unsigned int time = 0);
	void Stop();
	void Pause();
	void Unpause();
	void MemoryWarning();
	//void setSim(SimBase* sim){mCurrentSim = sim;};
	//SimBase* getSim(){return mCurrentSim;};
	GameID RegisterGameObject(GameObject* go);
	void AddTaggedObject(GameObject* g);
	void DeleteGameObject(GameID id);
	void MakePendingDeletions();
	//MessageManager& GetMessageManager(){return *mMessageManager;};
	Renderer& GetRenderer(){return *mRenderer;};
	void SetRenderer(Renderer* renderer){mRenderer = renderer;};
	GameObject* GetGameObject(GameID id){ return mGameObjectDB[id];};
	GameObject* GetTaggedObject(std::string& tag);
	static GameObject& GetGameObjectStatic(GameID id);
	PersistentDataManager& GetDataMgr(){return mPersistentDataMgr;};
	TimerManager& GetTimerManager(){return *mTimerManager;};
	PhysicsManager& GetPhysicsManager(){return *mPhysicsManager;};
	void SaveDataToXML(const char* filename);
	void LoadXMLData(const char* filename);
	void Config(PersistentData& storageData);
	static GameObject* CreateGameObject(PersistentData& storageData);
	Scene* GetActiveScene();
	void AddScene(Scene* s);

	void SendOutMessage(GameMessage& msg, GameID recipient);
	void BroadcastMessage(GameMessage& msg);
	void ProcessMessage(GameMessage& msg);
	
	inline static GameID GetNextGameID()
	{
		return ++mLastGameID;
	}
	
	void DumpDebugData();
	//void TestLua();
	//static void TestLuaCallback();
	
private:
	//Remove Copy Constructor and assignment operator
	Game		&operator =  (const Game &t) { printf("Game = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	Game(const Game &t){printf("Game COPY CTOR SHOULDN'T BE GETTING CALLED");};
	
protected:
	static Game*	mInstance;
	static GameID	mLastGameID;
	
	bool			mPaused;
	SimBase*		mCurrentSim;
	Renderer*		mRenderer;
	//MessageManager* mMessageManager;
	TimerManager*	mTimerManager;
	PhysicsManager* mPhysicsManager;
	
	
	std::map<GameID, GameObject*>		mGameObjectDB;
	std::map<std::string, GameObject*>		mGameObjectTagDB;//Don't delete the pointers in this map... it is just used to help lookup objects
	PersistentDataManager					mPersistentDataMgr;
	std::vector<Scene*>						mActiveScenes;
	std::vector<Scene*>						mAllScenes;
	std::list<GameID>					mPendingDeletions;
public:
	static IvVector3							mAcceleration;
	static IvVector3							mOrientation;
	static std::string							mMemoryState;
};

#endif //MAIN_SIM_H