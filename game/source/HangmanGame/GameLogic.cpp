#include "GameLogic.h"
#include "LetterManager.h"
#include "LetterChooser.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "WordProgressDisplay.h"
#include "HangedManDisplay.h"
#include "HangmanStateDisplay.h"
#include "ScratchDisplay.h"
#include "DropManager.h"
#include "Platform.h"
#include "Sequence.h"
#include "Sound.h"
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
GameLogic* GameLogic::mInstance = NULL;
////////////////////////////////////////////////////////////////////////////////
GameLogic& GameLogic::GetInstance()
{
	/*if(mInstance == NULL)
	{
		mInstance = new GameLogic();
	}*/
	return *mInstance;

}
////////////////////////////////////////////////////////////////////////////////
GameLogic::GameLogic()
{
	mCurrentWord = "ERROR";
	mWrongLetterCounter = 0;
	mActiveSequence = NULL;
	mHangmanStateDisplay = NULL;
	mScratchDisplay = NULL;
	mWaitingToStartNewWord = false;
	GameLogic::mInstance = this;
	mPlayingLostSequence = false;
}
////////////////////////////////////////////////////////////////////////////////
GameLogic::GameLogic(PersistentData& storageData):GameObject(storageData)
{
	GameLogic::mInstance = this;
	mCurrentWord = "ERROR";
	mWrongLetterCounter = 0;
	mActiveSequence = NULL;
	mHangmanStateDisplay = NULL;
	std::string wordListPath = storageData.GetProperty("WordList");
	std::string sep = storageData.GetProperty("Separator");

	mMaxWrongGuesses = storageData.GetNodeFromPath("LoseCondition")->GetPropertyI("MaxWrongGuesses");
	mMetaballGridTag = storageData.GetNodeFromPath("MetaballGrid")->GetProperty("Tag");
	mLetterChooserTag = storageData.GetNodeFromPath("LetterChooser")->GetProperty("Tag");
	mWordProgressDisplayTag = storageData.GetNodeFromPath("WordProgressDisplay")->GetProperty("Tag");
	//mHangedManDisplayTag = storageData.GetNodeFromPath("HangedManDisplay")->GetProperty("Tag");
	mHangmanStateDisplayTag = storageData.GetNodeFromPath("HangmanStateDisplay")->GetProperty("Tag");
	mScratchDisplayTag = storageData.GetNodeFromPath("ScratchDisplay")->GetProperty("Tag");
	mDropManagerTag = storageData.GetNodeFromPath("DropManager")->GetProperty("Tag");
	
	mStartGameSequenceTag = storageData.GetNodeFromPath("StartGameSequence")->GetProperty("Tag");
	mStartSequenceTag = storageData.GetNodeFromPath("StartLevelSequence")->GetProperty("Tag");
	mStartAfterLossSequenceTag = storageData.GetNodeFromPath("StartLevelAfterLossSequence")->GetProperty("Tag");
	mBadLetterSequenceTag = storageData.GetNodeFromPath("BadLetterSequence")->GetProperty("Tag");
	mGoodLetterSequenceTag = storageData.GetNodeFromPath("GoodLetterSequence")->GetProperty("Tag");
	mGoodWordSequenceTag = storageData.GetNodeFromPath("GoodWordSequence")->GetProperty("Tag");
	mLoseSequenceTag = storageData.GetNodeFromPath("LoseSequence")->GetProperty("Tag");	
	
	mWaitingToStartNewWord = false;
	mPlayingLostSequence = false;
	
	std::string soundFile = "sounds/wind_sfx.caff";
	std::string path;
	GetFullFilePathFromResource(soundFile, path);
	mWindSoundEffectID = loadSound((char*)path.c_str());
	
	soundFile = "sounds/water_slide_sfx.caff";
	GetFullFilePathFromResource(soundFile, path);
	mWaterSoundEffectID = loadSound((char*)path.c_str());
	
	//mSound = new Sound((char*)path.c_str());
	mCurrentWordIndex = 0;
	mShowLovePoem = true;
	LoadPlayerPrefs(&mCurrentWordIndex, &mShowLovePoem);
	ReadWordList(wordListPath);	
}
////////////////////////////////////////////////////////////////////////////////
GameLogic::~GameLogic()
{
	SavePlayerProgress(mCurrentWordIndex,mShowLovePoem);
	//SAFE_DELETE(mSound);
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::LevelLoaded()
{
	mWaitingToStartNewWord = false;
	SetAllowChoosing(false);
	mLastWrongLetterCount = 0;
	mShowLovePoem = true;

	GameObject* go = mGame.GetTaggedObject(mMetaballGridTag);
	if(go)
	{
		mMetaballGrid = static_cast<MetaballGrid*>(go);	
	}
	go = mGame.GetTaggedObject(mLetterChooserTag);
	if(go)
	{
		mLetterChooser = static_cast<LetterChooser*>(go);	
	}

	go = mGame.GetTaggedObject(mWordProgressDisplayTag);
	if(go)
	{
		mWordDisplay = static_cast<WordProgressDisplay*>(go);
	}
	//go = mGame.GetTaggedObject(mHangedManDisplayTag);
	//if(go)
	//{
	//	mHangedManDisplay = static_cast<HangedManDisplay*>(go);
	//}
	go = mGame.GetTaggedObject(mHangmanStateDisplayTag);
	if(go)
	{
		mHangmanStateDisplay = static_cast<HangmanStateDisplay*>(go);
	}
	go = mGame.GetTaggedObject(mScratchDisplayTag);
	if(go)
	{
		mScratchDisplay = static_cast<ScratchDisplay*>(go);
	}
	go = mGame.GetTaggedObject(mDropManagerTag);
	if(go)
	{
		mDropManager = static_cast<DropManager*>(go);
	}
	
	go = mGame.GetTaggedObject(mBadLetterSequenceTag);
	if(go)
	{
		mBadLetterSequence = static_cast<Sequence*>(go);
	}
	
	go = mGame.GetTaggedObject(mGoodLetterSequenceTag);
	if(go)
	{
		mGoodLetterSequence = static_cast<Sequence*>(go);
	}
	
	go = mGame.GetTaggedObject(mGoodWordSequenceTag);
	if(go)
	{
		mGoodWordSequence = static_cast<Sequence*>(go);
	}
	
	go = mGame.GetTaggedObject(mLoseSequenceTag);
	if(go)
	{
		mLoseSequence = static_cast<Sequence*>(go);
	}

	go = mGame.GetTaggedObject(mStartSequenceTag);
	if(go)
	{
		mStartSequence = static_cast<Sequence*>(go);
	}

	go = mGame.GetTaggedObject(mStartAfterLossSequenceTag);
	if(go)
	{
		mStartAfterLossSequence = static_cast<Sequence*>(go);
	}
	
	go = mGame.GetTaggedObject(mStartGameSequenceTag);
	if(go)
	{
		mStartGameSequence = static_cast<Sequence*>(go);
	}
	
	mLetterChooser->AddEventListener("ALL",mID);
	mWordDisplay->AddEventListener("ALL",mID);
	mBadLetterSequence->AddEventListener("ALL",mID);
	mGoodLetterSequence->AddEventListener("ALL",mID);
	mGoodWordSequence->AddEventListener("ALL",mID);
	mLoseSequence->AddEventListener("ALL",mID);
	mStartSequence->AddEventListener("ALL",mID);
	mStartAfterLossSequence->AddEventListener("ALL",mID);
	mStartGameSequence->AddEventListener("ALL",mID);

	mCanStartNextWord = true;
	
	mWordDisplay->ClearTiles();
	mLetterChooser->Clear();
	//mHangmanStateDisplay->SetState(0);
	mWrongLetterCounter =  -1;
	mStartGameSequence->StartAsSequenceItem();
	
	if(mCurrentWordIndex + 1 >= (int)mWordList.size())
		mCurrentWord = mWordList[0];
	else
		mCurrentWord = mWordList[mCurrentWordIndex];
	
	mCurrentWord = ToUpper(mCurrentWord);

}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::StartNextWord()
{
	mCanStartNextWord = false;
	mChosenLetters.clear();
	if(mWordList.empty() || mCurrentWordIndex >= (int)mWordList.size())
		return;
	
	mCurrentWord = mWordList[mCurrentWordIndex];
	mCurrentWord = ToUpper(mCurrentWord);
	mCurrentWordIndex++;
	ProcessChosenLetters();
	//mWordDisplay->SetTargetWord(mCurrentWord);
	//mMetaballGrid->ClearGridValues();
	mLetterChooser->Reset();
	
	char buff[256];
	sprintf(buff,"Mistakes: %d, Max: %d %s",mWrongLetterCounter,mMaxWrongGuesses,mCurrentWord.c_str());
	//mHangedManDisplay->SetDisplayText(buff);
	SetAllowChoosing(true);
	printf("%s\n",mCurrentWord.c_str());
	//mGame.DumpDebugData();
	//printf("---------------------------------------------------------------\n");
	//mGame.GetRenderer().DumpDebugData();
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ReadWordList(std::string filename)
{
	//printf("Loading %s\n",filename.c_str());
	std::string fullpath;
	std::string file = filename;
	GetFullFilePathFromResource(file,fullpath);

	
	std::string line;
	std::ifstream textstream(fullpath.c_str());
	while (std::getline(textstream, line)) 
	{
		mWordList.push_back(line);
	} 
	textstream.close();
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("GameLogic");

}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::SpawnDrops()
{
	mDropManager->Splash();
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ChangeBackground()
{
	mHangmanStateDisplay->SetState(mWrongLetterCounter+1);
	mScratchDisplay->SetState(mWrongLetterCounter+1);
	mLetterChooser->AdjustLayout();
	
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ChangeWord()
{
	if(mCurrentWordIndex + 1 >= (int)mWordList.size())
		mCurrentWord = mWordList[0];
	else
		mCurrentWord = mWordList[mCurrentWordIndex];
	
	mCurrentWord = ToUpper(mCurrentWord);
	
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ShowTiles()
{
	mWordDisplay->SetTargetWord(mCurrentWord);
	
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::SetBackground(int idx)
{
	mHangmanStateDisplay->SetState(idx);
	mScratchDisplay->SetState(idx);

}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ProcessMessage(GameMessage& msg)
{
	mLastMessage = msg.mEventName;
	
	if(msg.mEventName == "LETTER_CHOSEN")
	{
		mCanStartNextWord = true;
		LetterChooserMessage& m = static_cast<LetterChooserMessage&>(msg);
		//printf("Letter Chosen: %c\n",m.mLetterButton->mChar);
		std::string letter;
		letter += m.mLetterButton->mChar;
		mLastChosenLetter = letter.c_str()[0];
		mChosenLetters.push_back(letter);
		ProcessChosenLetters();
		//mWordDisplay->BlowTilesAround();
		//SetAllowChoosing(false);
		
		
		//playSound(mSoundEffectID);
		//mWordDisplay->SetTargetWord(mDisplayedWord);
		
	}
	else if(msg.mEventName == "SEQUENCE_FINISHED")
	{
		if(mActiveSequence != NULL)
		{
			printf("GameLogic::ProcessMessage got a message \"SEQUENCE_FINISHED\" and doesn't know what to do with it... did you forget to set the MessageOnFinish property in config.xml for the Sequence?\n");
		}
	}
	else if(msg.mEventName == "WRONG_SELECTION_DONE")
	{
		//Unlock the letter chooser
		mPlayingLostSequence = false;
		mLetterChooser->SetActive(true);
		//SetAllowChoosing(true);
	}
	else if(msg.mEventName == "CORRECT_SELECTION_DONE")
	{
		//Unlock the letter chooser
		//std::string letter = mChosenLetters.back();
		//char c = letter[0];
		//mWordDisplay->LetterChosen(c);
		mPlayingLostSequence = false;
		mLetterChooser->SetActive(true);
		
	}
	else if(msg.mEventName == "LOST_DONE")
	{
		//Restart the whole game???
		mPlayingLostSequence = false;
		mLetterChooser->SetActive(false);
		mLetterChooser->Clear();
		

		
		if(mShowLovePoem)
		{
			//AutoStart next word
			mCurrentWordIndex--;
			SavePlayerProgress(mCurrentWordIndex, mShowLovePoem);
			MouseDown(0,0,0);
		}
		else
		{
			ShowCorrectWord();
		}
		mWrongLetterCounter = 0;
		mWaitingToStartNewWord = true;
	}
	else if(msg.mEventName == "START_DONE" || msg.mEventName == "GAME_START_DONE")
	{
		//std::string letter = mChosenLetters.back();
		//char c = letter[0];
		//mWordDisplay->LetterChosen(c);
		
		//Hack to get around bug that I can't fix at the moment.. that GAME_START_DONE is being called twice... for some reason that sequence finishes twice... may be related to SprayAnimation
		if(mCanStartNextWord == false) return;
		mPlayingLostSequence = false;
		printf("START DONE\n");
		StartNextWord();
		//mLetterChooser->Reset();
		ChangeBackground();
	}
	//else if(msg.mEventName == "GAME_START_DONE")
	//{
	//	mWordDisplay->ClearTiles();
	//	mLetterChooser->Clear();
	//	mStartSequence->StartAsSequenceItem();
	//	
	//}
	else if(msg.mEventName == "CORRECT_WORD_DONE")
	{
		//mWordDisplay->ClearTiles();
		mPlayingLostSequence = false;
		mLetterChooser->Clear();
		mWaitingToStartNewWord = true;
		

		if(mShowLovePoem)
		{
			//AutoStart next word
			MouseDown(0,0,0);
		}
		
	}
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::MouseDown(float u, float v, int button)
{
	if(mWaitingToStartNewWord)
	{
		
		mWordDisplay->ClearTiles();
		//StartNextWord();
		mLetterChooser->NextClickActivates();
		mWaitingToStartNewWord = false;
		//mHangmanStateDisplay->SetState(0);
		if(mLastMessage == "LOST_DONE")//mPlayingLostSequence)
		{
			mStartAfterLossSequence->StartAsSequenceItem();
		}
		else
		{
			mStartSequence->StartAsSequenceItem();
		}
		//mLetterChooser->SetActive(false);
	}
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ProcessChosenLetters()
{
	int matchedLetters = 0;
	mDisplayedWord = "";
	for(int k = 0; k < (int)mCurrentWord.size(); ++k)
	{
		mDisplayedWord += "_";
	}

	mWrongLetterCounter = 0;
	for(int i = 0; i < (int)mChosenLetters.size(); ++i)
	{	
		bool letterMatched = false;
		for(int j = 0; j < (int)mCurrentWord.size(); ++j)
		{
			std::string letterInWord = "";
			letterInWord += mCurrentWord[j];
			if(mChosenLetters[i] == letterInWord)
			{
				letterMatched = true;
				mDisplayedWord[j] = mCurrentWord[j];
				matchedLetters++;
			}
		}
		if(!letterMatched)
			mWrongLetterCounter++;
	}

	bool madeABadChoice = false;
	if(mLastWrongLetterCount < mWrongLetterCounter)
	{
		madeABadChoice = true;
	}
	mLastWrongLetterCount = mWrongLetterCounter;


	char buff[256];
	sprintf(buff,"Mistakes: %d, Max: %d",mWrongLetterCounter,mMaxWrongGuesses);
	//mHangedManDisplay->SetDisplayText(buff);
	
	//printf("%d letters incorrect. %d is the limit\n",mWrongLetterCounter,mMaxWrongGuesses);


	if(mWrongLetterCounter > mMaxWrongGuesses)
	{
		//SetAllowChoosing(false);
		mWrongLetterCounter = 0;
		mPlayingLostSequence = true;
		SavePlayerProgress(mCurrentWordIndex,mShowLovePoem);
		mLoseSequence->StartAsSequenceItem();
		//mLetterChooser->SetActive(false);
	}
	else if(matchedLetters == (int)mCurrentWord.size())
	{
		mWrongLetterCounter = 0;
		SavePlayerProgress(mCurrentWordIndex,mShowLovePoem);
		//SetAllowChoosing(false);
		mGoodWordSequence->StartAsSequenceItem();
		//mLetterChooser->SetActive(false);
	}
	else if(madeABadChoice)
	{
		//SetAllowChoosing(false);
		mBadLetterSequence->StartAsSequenceItem();
		//mLetterChooser->SetActive(false);
	}
	else if(mChosenLetters.size() > 0)//chose a good letter or started level
	{
		//SetAllowChoosing(false);
		mGoodLetterSequence->StartAsSequenceItem();
		//mLetterChooser->SetActive(false);
	}
	
	
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::BlowTiles()
{
	mWordDisplay->BlowTilesAround();
	playSound(mWindSoundEffectID);
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ClearMetaballGrid()
{
	mMetaballGrid->ClearGridValues();
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ClearScreen()
{
	mMetaballGrid->ClearGridValues();
	mWordDisplay->ClearTiles();
	mLetterChooser->Clear();
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::ShowCorrectWord()
{
	mLetterChooser->Clear();
	mWordDisplay->ShowWholeWord(mCurrentWord);
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::StopSoundEffect(std::string effectName)
{
	if(effectName == "WIND")
	{
		//stopSound(mWindSoundEffectID,true);
	}
	else
	{
		stopSound(mWaterSoundEffectID,true);
	}
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::StartSoundEffect(std::string effectName)
{
	
	if(effectName == "WIND")
	{
		playSound(mWindSoundEffectID);
	}
	else
	{
		playSound(mWaterSoundEffectID);
	}
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::SetAllowChoosing(bool b)
{
	mAllowChoosing = b;
}
////////////////////////////////////////////////////////////////////////////////
void GameLogic::GetBlockedDropletAreas(std::vector<IvAABB>& aabbs)
{
	if(mHangmanStateDisplay == NULL)
		return;
	
	mHangmanStateDisplay->GetBlockedDropletAreas(aabbs,mWrongLetterCounter+1);
}
////////////////////////////////////////////////////////////////////////////////
					



