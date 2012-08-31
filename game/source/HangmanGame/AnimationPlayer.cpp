#include "AnimationPlayer.h"
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


////////////////////////////////////////////////////////////////////////////////
AnimationPlayer::AnimationPlayer(PersistentData& storageData):RenderObject(storageData)
{
	mAnimation = NULL;
	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	
	PersistentData* node = storageData.GetNodeFromPath("Foreground.RenderPlane");
	mForeground = static_cast<RenderPlane*>(GameObjectFactory::CreateGameObject(*node));
	node = storageData.GetNodeFromPath("Background.RenderPlane");
	mBackground = static_cast<RenderPlane*>(GameObjectFactory::CreateGameObject(*node));
	
	node = storageData.GetNodeFromPath("Animation");
	if(node != NULL)
	{
		mAnimationPosition = node->GetVector3Child("Position");
	}

	AddChild(mForeground);
	AddChild(mBackground);
}
////////////////////////////////////////////////////////////////////////////////
AnimationPlayer::~AnimationPlayer()
{
	
}

////////////////////////////////////////////////////////////////////////////////
void AnimationPlayer::LevelLoaded()
{

	mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));

}
////////////////////////////////////////////////////////////////////////////////
void AnimationPlayer::ProcessMessage(GameMessage& msg)
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
		
	}
}

////////////////////////////////////////////////////////////////////////////////
void AnimationPlayer::Update()
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
void AnimationPlayer::StartAsSequenceItem(PersistentData* storageData)
{
	mForeground->SetActive(true);
	mForeground->SetVisible(true);
	mBackground->SetActive(true);
	mBackground->SetVisible(true);
	bool chooseRandom = storageData->GetPropertyB("ChooseRandom");
	
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
	if(nodeCount > 1 && chooseRandom == false)
	{
		r = mGameLogic->GetWrongLetterCount() - 1;
		
		//child = *(nodes.begin());	
	}
	else
	{
		r = rand() % nodeCount;
		/*it = nodes.begin();
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
		}*/
	}
	
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
	mAnimation->StartAsSequenceItem();
}
////////////////////////////////////////////////////////////////////////////////
void AnimationPlayer::StopAsSequenceItem()
{
	mActive = false;
	mVisible = false;
	mForeground->SetActive(false);
	mForeground->SetVisible(false);
	mBackground->SetActive(false);
	mBackground->SetVisible(false);
	//RemoveChild(mForeground);
	//RemoveChild(mBackground);
	//mGame.DeleteGameObject(mForeground->GetID());
	//mGame.DeleteGameObject(mBackground->GetID());
	
	mAnimation->RemoveEventListener("FINISHED",mID);
	mAnimation->StopAsSequenceItem();
	RemoveChild(mAnimation);
	mGameLogic->ChangeBackground();
	
}
////////////////////////////////////////////////////////////////////////////////
std::string AnimationPlayer::DebugOutput()
{
	char buf[256];
	std::string animName = "None";
	if(mAnimation != NULL)
	{
		animName = mAnimation->GetAnimationName();
	}
	
	unsigned int fgID = 0;
	if(mForeground != NULL)
		fgID = (unsigned int)mForeground->GetID();
	sprintf(buf, "\tAnimationName:%s\tForground:%d",animName.c_str(),fgID);
	std::string superInfo = RenderObject::DebugOutput();
	std::string s = superInfo + buf;
	return s;
}
////////////////////////////////////////////////////////////////////////////////