#include "LetterSprayAnimation.h"
#include "LetterDisplayAnimation.h"
#include "WordProgressDisplay.h"
#include "LetterChooser.h"
#include "Game.h"
#include "GameLogic.h"
#include "Platform.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "MetaballManager2D.h"
#include "Scene.h"
////////////////////////////////////////////////////////////////////////////////
LetterSprayAnimation::LetterSprayAnimation(PersistentData& storageData):GameObject(storageData)
{
	std::string fontName = storageData.GetProperty("Font");
	GameID fontID = RendererObjectFactory::CreateTextureFont(fontName.c_str());
	TextureFont& font = RendererObjectFactory::GetTextureFont(fontID);
	mFont = &font;
	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	mMetaballGridTag = storageData.GetProperty("TargetMetaballGrid");
	mMetaballMgr2DTag = storageData.GetProperty("TargetMetaballManager2DTag");
	if(storageData.GetPropertyB("RefsAreImmediate"))
	{
		mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
		mMetaballGrid = static_cast<MetaballGrid*>(mGame.GetTaggedObject(mMetaballGridTag));
		mMetaballMgr2D = static_cast<MetaballManager2D*>(mGame.GetTaggedObject(mMetaballMgr2DTag));
	}
	mDuration = storageData.GetPropertyU("Duration");
	mChar = '_';
}
////////////////////////////////////////////////////////////////////////////////
LetterSprayAnimation::~LetterSprayAnimation()
{
	Cleanup();
}

////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("LetterSprayAnimation");
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::Update()
{
	/*
	if(!mActive)
		return;

	std::list<LetterDisplayAnimation*>::iterator it = mLetterAnims.begin();
	while(it != mLetterAnims.end())
	{
		(*it)->Update();
		if(mActive == false)
			break;
		++it;
	}
	*/
}

////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::ProcessMessage(GameMessage& msg)
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

		int numActive = 0;
		std::list<LetterDisplayAnimation*>::iterator laIt = mLetterAnims.begin();
		while(laIt != mLetterAnims.end())
		{
			if((*laIt)->IsActive())
				numActive++;

			++laIt;
		}
		

		if(numActive == 0)
		{
			GameMessage msg;
			msg.mEventName = "FINISHED";
			msg.mSender = mID;
			SendOutMessage(msg);
			//StopAsSequenceItem();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::StartAsSequenceItem(PersistentData* storageData)
{		
	std::vector<IvVector3> endPositions;
	std::vector<IvVector3> startPositions;
	LetterChooser& lc = mGameLogic->GetLetterChooser();
	lc.CalculatePositions();
	lc.GetLetterPositions(endPositions);
	IvVector3 pos;
	pos.Set(160,-50,100);


	for(unsigned int i = 0; i < endPositions.size(); ++i)
	{
		startPositions.push_back(pos);
	}
	
	mStartPositions = startPositions;
	mEndPositions = endPositions;
	mActive = true;

	std::vector<char> chars;
	lc.GetChars(chars);
	for(unsigned int i = 0; i < mStartPositions.size(); ++i)
	{
		IvVector3 start = mStartPositions[i];
		IvVector3 end = mEndPositions[i];
		char c = chars[i];
		LetterDisplayAnimation* lda = new LetterDisplayAnimation(mFont,mMetaballGrid,mMetaballMgr2D,c,start, end,mDuration);
		mLetterAnims.push_back(lda);
		lda->AddEventListener("FINISHED",mID);
		lda->StartAsSequenceItem();
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::StopAsSequenceItem()
{
	mActive = false;
	//mGame.GetActiveScene()->RemoveGameObject(this);
	Cleanup();
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterSprayAnimation::Cleanup()
{
	std::list<LetterDisplayAnimation*>::iterator it = mLetterAnims.begin();
	while(it != mLetterAnims.end())
	{
		mGame.DeleteGameObject((*it)->GetID());
		++it;
	}
	mLetterAnims.clear();
}
////////////////////////////////////////////////////////////////////////////////