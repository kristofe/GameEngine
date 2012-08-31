/*
 *  Scene.cpp
 *  GWEngine_1.0
 *
 *  Created by Kristofer Schlachter on 12/26/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */
#include "Platform.h"
#include "Game.h"
#include "Globals.h"

#include "Scene.h"
#include "Camera.h"
#include "RenderPlane.h"
#include "WaveGrid.h"
#include "StamFluidSolver.h"
#include "MetaballGrid.h"
#include "TextureFont.h"
#include "TextureText.h"
#include "RendererFPSDisplay.h"
#include "AnimatedTexture.h"
#include "PipelineTools.h"
#include "PersistentDataManager.h"
////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	mPaused = true;
}
////////////////////////////////////////////////////////////////////////////////
Scene::Scene(PersistentData& storageData)
{
	mActive = storageData.GetPropertyB("Active");
	mName = storageData.GetProperty("Name");	
	Game::GetInstance().AddScene(this);
	
	PersistentData* renderTargets = storageData.GetNodeFromPath("RenderTargets");
	if(renderTargets != NULL)
	{
		std::list<PersistentData*> children = renderTargets->GetChildren();
		std::list<PersistentData*>::iterator it = children.begin();
		while(it != children.end())
		{
			PersistentData* c = (*it);
			GameObject* go = Game::CreateGameObject(*c);
			if(go != NULL)
			{
				mGameObjects[go->GetID()] = go;
			}
			++it;
		}
		
	}
	
	Renderer& r = Game::GetInstance().GetRenderer();
	
	PersistentData* cameras = storageData.GetNodeFromPath("Cameras");
	if(cameras != NULL)
	{
		std::list<PersistentData*> children = cameras->GetChildren();
		std::list<PersistentData*>::iterator it = children.begin();
		while(it != children.end())
		{
			PersistentData* c = (*it);
			GameObject* go = Game::CreateGameObject(*c);
			if(go != NULL)
			{
				r.AddCamera((Camera*)go);
				mGameObjects[go->GetID()] = go;
			}
			++it;
		}
		
	}
	
	PersistentData* gameObjects = storageData.GetNodeFromPath("GameObjects");
	std::list<PersistentData*> children = gameObjects->GetChildren();
	std::list<PersistentData*>::iterator it = children.begin();
	while(it != children.end())
	{
		PersistentData* c = (*it);
		GameObject* go = Game::CreateGameObject(*c);
		if(go != NULL)
		{
			mGameObjects[go->GetID()] = go;
		}
		++it;
	}
	std::map<GameID,GameObject*>::iterator git = mGameObjects.begin();
	while(git != mGameObjects.end())
	{
		git->second->LevelLoaded();
		++git;
	}
}
////////////////////////////////////////////////////////////////////////////////
Scene::~Scene()
{

}
////////////////////////////////////////////////////////////////////////////////
void Scene::Start(unsigned int time){
	mPaused = false;
	mStartTime = time;
	mCurrentTime = time;
	mDuration = 2000;
	std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
	while(it != mGameObjects.end())
	{
		(*it).second->LevelStart();
		++it;
	}

	//Game::GetInstance().SaveDataToXML("SaveFile.xml");
}
////////////////////////////////////////////////////////////////////////////////
void Scene::Update(){
	if(!mPaused){
		MakePendingRemovals();
		std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
		while(it != mGameObjects.end())
		{
			(*it).second->Update();
			++it;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Scene::LateUpdate(){
	if(!mPaused){
		std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
		while(it != mGameObjects.end())
		{
			(*it).second->LateUpdate();
			++it;
		}

	}
}
////////////////////////////////////////////////////////////////////////////////
void Scene::FixedUpdate(){
	if(!mPaused){
		std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
		while(it != mGameObjects.end())
		{
			(*it).second->FixedUpdate();
			++it;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Scene::MouseUp(float u, float v, int button){
	if(!mPaused){
		std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
		while(it != mGameObjects.end())
		{
			(*it).second->MouseUp(u,v,button);
			++it;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Scene::MouseDown(float u, float v, int button){
	if(!mPaused){
		std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
		while(it != mGameObjects.end())
		{
			(*it).second->MouseDown(u,v,button);
			++it;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Scene::MouseDragged(float u, float v,float lu, float lv, int button){
	if(!mPaused){
		std::map<GameID,GameObject*>::iterator it = mGameObjects.begin();
		while(it != mGameObjects.end())
		{
			(*it).second->MouseDragged(u,v,lu,lv,button);;
			++it;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Scene::AddGameObject(GameObject* go)
{
	mGameObjects[go->GetID()] = go;
}
////////////////////////////////////////////////////////////////////////////////
void Scene::RemoveGameObject(GameID id)
{
	mGameObjectsToRemove.push_back(id);
}
////////////////////////////////////////////////////////////////////////////////
void Scene::MakePendingRemovals()
{
	std::list<GameID>::iterator rit = mGameObjectsToRemove.begin();
	while(rit != mGameObjectsToRemove.end())
	{
		std::map<GameID,GameObject*>::iterator it = mGameObjects.find(*rit);
		if(it != mGameObjects.end())
		{
			it->second = NULL;
			mGameObjects.erase(it);
		}
		++rit;
	}
	mGameObjectsToRemove.clear();
}
////////////////////////////////////////////////////////////////////////////////

