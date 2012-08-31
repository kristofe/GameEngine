/*
 *  Game.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 12/25/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */
#include "Renderer.h"
#include "Game.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameObjectFactory.h"
#include "MessageManager.h"
#include "LayerManager.h"
#include "TimerManager.h"
#include "PhysicsManager.h"
#include "Platform.h"

#include <stdio.h>
extern "C"{
#include "lua.h" 
#include "lualib.h" 
#include "lauxlib.h"

extern int luaopen_example(lua_State* L); // declare the wrapped module
};
#define LUA_EXTRALIBS {"example",luaopen_example}



////////////////////////////////////////////////////////////////////////////////
std::string	Game::mMemoryState = "";
IvVector3 Game::mAcceleration;
IvVector3 Game::mOrientation;
Game* Game::mInstance = NULL;
GameID Game::mLastGameID = 0;
////////////////////////////////////////////////////////////////////////////////
Game& Game::GetInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new Game();
	}
	return *mInstance;

}

////////////////////////////////////////////////////////////////////////////////
GameObject& Game::GetGameObjectStatic(GameID id)
{
	return *(GetInstance().GetGameObject(id));

}
////////////////////////////////////////////////////////////////////////////////
Game::Game(){
	mPaused = true;
	mCurrentSim = NULL;
	Time::Init();
	mPhysicsManager = new PhysicsManager(*this);
	//mTimerManager = new TimerManager();
	//mMessageManager = new MessageManager();
}
////////////////////////////////////////////////////////////////////////////////
Game::~Game(){
	SAFE_DELETE(mPhysicsManager);
	SAFE_DELETE(mCurrentSim);
}
////////////////////////////////////////////////////////////////////////////////
void Game::Start(unsigned int time){
	mPaused = false;
	Time::Start();
	mPhysicsManager->Start(time);
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->Start(time);
	}
	TestLua();
}
////////////////////////////////////////////////////////////////////////////////
void Game::Stop(){
	mPaused = true;
	Time::Stop();
	mPhysicsManager->Stop();
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->Stop();
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::Pause(){
	mPaused = true;
	Time::Pause(true);
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->Pause();
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::Unpause(){
	mPaused = false;
	Time::Pause(false);
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->Unpause();
	}
}
////////////////////////////////////////////////////////////////////////////////

void Game::Update(){
	MakePendingDeletions();
	Time::Update();
	mPhysicsManager->Update();
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->Update();
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::FixedUpdate(){
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->FixedUpdate();
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::LateUpdate(){
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->LateUpdate();
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::MouseDown(float u, float v, int button){
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->MouseDown(u,v,button);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::MouseDragged(float u, float v,float lu, float lv, int button){
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->MouseDragged(u,v,lu,lv,button);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::MouseUp(float u, float v, int button){
	for(int i = 0; i < (int)mActiveScenes.size(); ++i)
	{
		mActiveScenes[i]->MouseUp(u,v,button);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::AddTaggedObject(GameObject* go)
{
	mGameObjectTagDB[go->GetTag()] = go;
}
////////////////////////////////////////////////////////////////////////////////
GameObject* Game::GetTaggedObject(std::string& tag)
{
	std::map<std::string, GameObject*>::iterator it = mGameObjectTagDB.find(tag);
	if(it != mGameObjectTagDB.end())
		return (*it).second;
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
GameID Game::RegisterGameObject(GameObject* go){
	GameID id = Game::GetNextGameID();
	mGameObjectDB[id] = go;
	go->SetID(id);
	GetActiveScene()->AddGameObject(go);
	return id;
}
////////////////////////////////////////////////////////////////////////////////
void Game::DeleteGameObject(GameID id){
	GameObject* go = mGameObjectDB[id];
	go->AddedToDeletionList();
	mPendingDeletions.push_back(id);
}
////////////////////////////////////////////////////////////////////////////////
void Game::MakePendingDeletions(){
	std::list<GameID>::iterator it = mPendingDeletions.begin();
	while(it != mPendingDeletions.end())
	{
		GameID id = *it;
		std::map<GameID, GameObject*>::iterator dbEntry = mGameObjectDB.find(id);
		GameObject* go = dbEntry->second;
		for(int i = 0; i < (int)mActiveScenes.size(); ++i)
		{
			mActiveScenes[i]->RemoveGameObject(id);
		}
		if(go != NULL)
		{
			std::map<std::string, GameObject*>::iterator tagIt = mGameObjectTagDB.find(go->GetTag());
			if(tagIt != mGameObjectTagDB.end())
				mGameObjectTagDB.erase(tagIt);
		}
		
		SAFE_DELETE(go);
		if(dbEntry != mGameObjectDB.end())
			mGameObjectDB.erase(dbEntry);
		
		++it;
	}
	mPendingDeletions.clear();
}
////////////////////////////////////////////////////////////////////////////////
void Game::LoadXMLData(const char* filename)
{
	mPersistentDataMgr.LoadXMLFile("Config.xml");
	Config(*(mPersistentDataMgr.GetDataTree().GetRoot()));
	
}
////////////////////////////////////////////////////////////////////////////////
GameObject* Game::CreateGameObject(PersistentData& storageData)
{
	return GameObjectFactory::CreateGameObject(storageData);
	/*
	std::string objType = storageData.GetName();
	GameObject* go = NULL;
	if(objType == "RenderPlane")
	{
		go = new RenderPlane(storageData);
	}
	else if(objType == "RendererFPSDisplay")
	{
		go = new RendererFPSDisplay(storageData);
	}
	else if(objType == "MetaballGrid")
	{
		go = new MetaballGrid(storageData);
	}
	else if(objType == "LetterManager")
	{
		go = new LetterManager(storageData);
	}
	
	return go;*/
	
}
////////////////////////////////////////////////////////////////////////////////
void Game::Config(PersistentData& storageData)
{
	PersistentData* c;
	c = storageData.GetNodeFromPath("Game.Layers");
	new LayerManager(*c);//Singleton
	
	std::list<PersistentData*> children;
	std::list<PersistentData*>::iterator it;
	storageData.GetNodesFromPath("Game.Scenes.Scene",children);
	it = children.begin();
	while(it != children.end())
	{
		c = (*it);
		new Scene(*c);
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////
Scene* Game::GetActiveScene()
{
	if(mActiveScenes.empty())
		return NULL;

	return mActiveScenes.back();

}
////////////////////////////////////////////////////////////////////////////////
void Game::AddScene(Scene* scene)
{
	mAllScenes.push_back(scene);
	if(scene->IsActive())
	{
		mActiveScenes.push_back(scene);
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void Game::SaveDataToXML(const char* filename)
{
	PersistentData* root = mPersistentDataMgr.GetDataTree().GetRoot();
	if(root == NULL)
	{
		root = new PersistentData("root","root");
		mPersistentDataMgr.GetDataTree().AddNode(NULL, root);
	}
	PersistentData* saveRoot = new PersistentData("SavedGame");
	//root->AddChild(saveRoot);
	
	PersistentData* rendererData = new PersistentData("Renderer");
	mRenderer->Serialize(*rendererData);
	saveRoot->AddChild(rendererData);
	
	std::map<GameID, GameObject*>::iterator it = mGameObjectDB.begin();
	while(it != mGameObjectDB.end())
	{
		
		GameObject* go = (*it).second;
		PersistentData* data = new PersistentData("GameObject");
		saveRoot->AddChild(data);
		go->Serialize(*data);
		++it;
	}
	mPersistentDataMgr.SaveXMLFile("SAVEFILE.xml",saveRoot);
	//root->RemoveChild(saveRoot);
	mPersistentDataMgr.GetDataTree().DeleteSubTree(saveRoot);
}
////////////////////////////////////////////////////////////////////////////////
void Game::SendOutMessage(GameMessage& msg, GameID recipient)
{
	GameObject* go = GetGameObject(recipient);
	if(go)
	{
		go->ProcessMessage(msg);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::BroadcastMessage(GameMessage& msg)
{
	std::map<GameID, GameObject*>::iterator it = mGameObjectDB.begin();
	while(it != mGameObjectDB.end())
	{
		it->second->ProcessMessage(msg);
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::ProcessMessage(GameMessage& msg)
{
	std::string eventName = ToUpper(msg.mEventName);
	if(eventName == "TOO_MANY_GUESSES")
	{
		printf("TOO MANY GUESSES!\n");
	}
	else if(eventName == "CORRECTLY_GUESSED_WORD")
	{
		printf("CORRECTLY GUESSED WORD!\n");
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::UpdateAccelerometerAndOrientation(const IvVector3& accel, const IvVector3& orient)
{ 
	Game::mAcceleration = accel; 
	Game::mOrientation = orient;
}
////////////////////////////////////////////////////////////////////////////////
void Game::MemoryWarning()
{ 
	printf("WARNING! Received low memory warning from operating system!!!!!!!!\n");
	Game::mMemoryState = "WARNING! LOW MEMORY! "; 
}
////////////////////////////////////////////////////////////////////////////////
void Game::DumpDebugData()
{ 
	std::map<GameID, GameObject*>::iterator it = mGameObjectDB.begin();
	while(it != mGameObjectDB.end())
	{
		printf("%s\n",it->second->DebugOutput().c_str());
		++it;
	} 
	
	std::list<GameID>::iterator it2 = mPendingDeletions.begin();
	while(it2 != mPendingDeletions.end())
	{
		printf("%d\tPendingDeletion\n",(int)(*it2));
		++it2;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Game::TestLua()
{
	lua_State *L;
	std::string fullpath;
	std::string file = "example.lua";
	GetFullFilePathFromResource(file,fullpath);
	printf("TestLua\n");
	L=lua_open(); 
	luaopen_base(L);	// load basic libs (eg. print) 
	luaopen_example(L);	// load the wrappered module
	if (luaL_loadfile(L,fullpath.c_str())==0) // load and run the file
		lua_pcall(L,0,0,0); 
	else
		printf("unable to load %s\n",fullpath.c_str()); 
	lua_close(L);
	
}
////////////////////////////////////////////////////////////////////////////////
void Game::TestLuaCallback()
{
		printf("In TestLuaCallback() !!!!\n");
}
////////////////////////////////////////////////////////////////////////////////
