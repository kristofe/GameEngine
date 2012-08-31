/*
 *  Scene.h
 *  GWEngine_1.0
 *
 *  Created by Kristofer Schlachter on 12/26/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */

#ifndef _SCENE_H
#define _SCENE_H

#include "SimBase.h"
class GameObject;
class PersistentData;

class Scene : public SimBase{
public:
	Scene();
	Scene(PersistentData& storageData);
	~Scene();
	
	void Update();
	void LateUpdate();
	void FixedUpdate();
	void MouseDown(float u, float v, int button);
	void MouseDragged(float u, float v,float lu,float lv, int button);
	void MouseUp(float u, float v, int button);
	//void updateAccelerometer(float u, float v,float w);
	void Start(unsigned int time = 0);

	void AddGameObject(GameObject* go);
	void RemoveGameObject(GameID id);
	void MakePendingRemovals();
	
	virtual std::string GetTypeString()
	{
		return "Scene";
	}
	//void Config(PersistentData& storageData);
	
	std::string GetName(){return mName;};
	bool IsActive(){return mActive;};
	
protected:
	unsigned int mStartTime;
	unsigned int mElapsedTime;
	unsigned int mDuration;
	unsigned int mCurrentTime;
	bool mPaused;
	bool mActive;
	std::string mName;
	std::map<GameID,GameObject*> mGameObjects;
	std::list<GameID> mGameObjectsToRemove;
	
};

#endif //_SCENE_H