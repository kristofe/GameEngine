#include "LetterDripAnimation.h"
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
LetterDripAnimation::LetterDripAnimation(PersistentData& storageData):GameObject(storageData)
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
LetterDripAnimation::~LetterDripAnimation()
{
	Cleanup();
}

////////////////////////////////////////////////////////////////////////////////
void LetterDripAnimation::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("LetterDripAnimation");
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDripAnimation::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDripAnimation::Update()
{
	/*if(!mActive)
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
void LetterDripAnimation::ProcessMessage(GameMessage& msg)
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
void LetterDripAnimation::StartAsSequenceItem(PersistentData* storageData)
{		
	//mGame.GetActiveScene()->AddGameObject(this);
	LetterButton* b = mGameLogic->GetLetterChooser().GetLastChosenLetter();
	char c = b->mChar;

	IvVector3 pos = b->GetPosition();
	pos.z = 100;
	std::vector<IvVector3> endPositions;
	std::vector<IvVector3> startPositions;
	//mGameLogic->GetWordProgressDisplay()->GetLetterPositions(c,endPositions);

	if(endPositions.empty())
	{
		IvVector3 endPos = pos;
		endPos.y = -50.0f;
		endPositions.push_back(endPos);
	}

	for(unsigned int i = 0; i < endPositions.size(); ++i)
	{
		startPositions.push_back(pos);
	}
	
	mChar = c;
	mStartPositions = startPositions;
	mEndPositions = endPositions;
	mActive = true;

	for(unsigned int i = 0; i < mStartPositions.size(); ++i)
	{
		IvVector3 start = mStartPositions[i];
		IvVector3 end = mEndPositions[i];
		LetterDisplayAnimation* lda = new LetterDisplayAnimation(mFont,mMetaballGrid,mMetaballMgr2D, mChar,start, end,mDuration);
		lda->SetLayerID(mLayerID);
		mLetterAnims.push_back(lda);
		lda->AddEventListener("FINISHED",mID);
		lda->StartAsSequenceItem();
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDripAnimation::StopAsSequenceItem()
{
	mActive = false;
	GameLogic::GetInstance().StopSoundEffect();
	//mGame.GetActiveScene()->RemoveGameObject(this);
	Cleanup();
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDripAnimation::Cleanup()
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