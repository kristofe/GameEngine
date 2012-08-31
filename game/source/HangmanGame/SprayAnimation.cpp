#include "SprayAnimation.h"
#include "Game.h"
#include "AnimatedTexture.h"
#include "Platform.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObjectFactory.h"
#include "RenderPlane.h"
#include "GameLogic.h"
#include "LetterChooser.h"

////////////////////////////////////////////////////////////////////////////////
SprayAnimation::SprayAnimation(PersistentData& storageData):RenderObject(storageData)
{
	//PersistentData* node = storageData.GetNodeFromPath("Foreground.RenderPlane");
	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	
	PersistentData* node = storageData.GetNodeFromPath("Animation");
	if(node != NULL)
	{
		mAnimationPosition = node->GetVector3Child("Position");
	}
	
}
////////////////////////////////////////////////////////////////////////////////
SprayAnimation::~SprayAnimation()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void SprayAnimation::LevelLoaded()
{
	
	mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	
}
////////////////////////////////////////////////////////////////////////////////
void SprayAnimation::ProcessMessage(GameMessage& msg)
{
	if(msg.mEventName == "FINISHED")
	{
		GameObject* go = mGame.GetGameObject(msg.mSender);
		if(go != NULL)
		{
			//This won't work because the listeners are still being iterated through!!
			go->RemoveEventListener("FINISHED",mID);
			go->StopAsSequenceItem();
			
		}
		
		
		GameMessage msg;
		msg.mEventName = "FINISHED";
		msg.mSender = mID;
		SendOutMessage(msg);
		return;
		
	}
	else if(msg.mEventName == "HALF_WAY_POINT")
	{
		mGameLogic->SpawnDrops();
		mGameLogic->GetLetterChooser().Reset();
		mGameLogic->ChangeWord();
		mGameLogic->ShowTiles();
		
		
	}
}

////////////////////////////////////////////////////////////////////////////////
void SprayAnimation::Update()
{
	/*if(mPaused || !mActive) return;
	 Animate();
	 
	 if(mDataLoaded)
	 {
	 Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	 m.Lock();
	 m.CopyInUVs(mUV);
	 m.Unlock();
	 }*/
}



////////////////////////////////////////////////////////////////////////////////
void SprayAnimation::StartAsSequenceItem(PersistentData* storageData)
{
	
	std::list<PersistentData*> nodes;
	storageData->GetChildren("Animation",nodes);
	int nodeCount = 0;
	std::list<PersistentData*>::iterator it = nodes.begin();
	while(it != nodes.end())
	{
		nodeCount++;
		++it;
	}
	
	int r = 0;
	PersistentData* child;
	it = nodes.begin();
	int i = 0;
	while(it != nodes.end())
	{
		
		if(i == r)
		{
			child = *it;
			break;
		}
		i++;
		++it;
	}
	
	std::string parameter = child->GetProperty("Tag");
	mAnimation = static_cast<AnimatedTexture*>(mGame.GetTaggedObject(parameter));
	
	IvVector3 scale;
	mAnimation->GetLocalScale(scale);
	mAnimation->ResetLocalTransform();
	
	mAnimation->SetPosition(mAnimationPosition);
	mAnimation->Scale(scale);
	AddChild(mAnimation);
	mActive = true;
	mVisible = true;
	mAnimation->AddEventListener("FINISHED",mID);
	mAnimation->AddEventListener("HALF_WAY_POINT",mID);
	mAnimation->StartAsSequenceItem();
	mGameLogic->StartSoundEffect("WATER");
}
////////////////////////////////////////////////////////////////////////////////
void SprayAnimation::StopAsSequenceItem()
{
	mActive = false;
	mVisible = false;
	mAnimation->SetActive(false);
	mAnimation->RemoveEventListener("FINISHED",mID);
	mAnimation->RemoveEventListener("HALF_WAY_POINT",mID);
	mAnimation->StopAsSequenceItem();
	RemoveChild(mAnimation);
}
////////////////////////////////////////////////////////////////////////////////
std::string SprayAnimation::DebugOutput()
{
	char buf[256];
	std::string animName = "None";
	unsigned int currID = 0;
	if(mAnimation != NULL)
	{
		animName = mAnimation->GetAnimationData().animationName;
		currID = (unsigned int)mAnimation->GetID();
	}
	sprintf(buf, "\tAnimatedTextureName:%s\tAnimatedTextureID:%d",animName.c_str(),currID);
	std::string superInfo = RenderObject::DebugOutput();
	std::string s = superInfo + buf;
	return s;
}
////////////////////////////////////////////////////////////////////////////////


