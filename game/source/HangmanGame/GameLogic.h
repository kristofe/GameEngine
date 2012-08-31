#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include "GameObject.h"


class Game;
class LetterChooser;
class WordProgressDisplay;
class HangedManDisplay;
class HangmanStateDisplay;
class Sequence;
class Sound;
class IvAABB;
class ScratchDisplay;
class MetaballGrid;
class DropManager;

class GameLogic : public GameObject
{
protected:
	MetaballGrid*				mMetaballGrid;
	LetterChooser*				mLetterChooser;
	WordProgressDisplay*		mWordDisplay;
	//HangedManDisplay*			mHangedManDisplay;
	HangmanStateDisplay*		mHangmanStateDisplay;
	ScratchDisplay*				mScratchDisplay;
	DropManager*				mDropManager;
	std::vector<std::string>	mWordList;
	int							mCurrentWordIndex;
	std::vector<std::string>	mChosenLetters;
	std::string					mCurrentWord;
	std::string					mDisplayedWord;
	char						mLastChosenLetter;
	int							mWrongLetterCounter;
	int							mLastWrongLetterCount;
	int							mMaxWrongGuesses;

	bool						mShowLovePoem;
	bool						mWaitingToStartNewWord;
	bool						mAllowChoosing;
	std::string					mMetaballGridTag;
	std::string					mLetterChooserTag;
	std::string					mWordProgressDisplayTag;
	//std::string					mHangedManDisplayTag;
	std::string					mHangmanStateDisplayTag;
	std::string					mScratchDisplayTag;
	std::string					mDropManagerTag;

	std::string					mStartGameSequenceTag;
	std::string					mStartSequenceTag;
	std::string					mStartAfterLossSequenceTag;
	std::string					mBadLetterSequenceTag;
	std::string					mGoodLetterSequenceTag;
	std::string					mGoodWordSequenceTag;
	std::string					mLoseSequenceTag;

	Sequence*					mStartGameSequence;
	Sequence*					mStartSequence;
	Sequence*					mStartAfterLossSequence;
	Sequence*					mBadLetterSequence;
	Sequence*					mGoodLetterSequence;
	Sequence*					mGoodWordSequence;
	Sequence*					mLoseSequence;
	Sequence*					mActiveSequence;
	
	unsigned long				mWindSoundEffectID;
	unsigned long				mWaterSoundEffectID;
	
	//UGLY HACK
	bool						mCanStartNextWord;
	bool						mPlayingLostSequence;
	std::string					mLastMessage;
public:
	static GameLogic& GetInstance();
	GameLogic();
	GameLogic(PersistentData& storageData);
	~GameLogic();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	virtual void ProcessMessage(GameMessage& msg);
	void LevelLoaded();
	void MouseDown(float u, float v, int button);

	char					GetLastChosenLetter(){return mLastChosenLetter;};
	LetterChooser&			GetLetterChooser(){return *mLetterChooser;};
	WordProgressDisplay*	GetWordProgressDisplay(){return mWordDisplay;};
	void					ChangeBackground();
	void					ChangeWord();
	void					ShowTiles();
	void					SetBackground(int idx);
	bool					AllowChoosing(){return mAllowChoosing;};
	void					SetAllowChoosing(bool v);
	void					StopSoundEffect(std::string effectName = "WIND");//{mSound->stop(true);};
	void					StartSoundEffect(std::string effectName = "WIND");//{mSound->play();};
	void					ShowCorrectWord();
	void					BlowTiles();
	void					ClearMetaballGrid();
	void					ClearScreen();
	void					SpawnDrops();
	bool					ShouldShowLovePoem(){return mShowLovePoem;};
	void					SetShouldShowLovePoem(bool b){mShowLovePoem = b;};
	int						GetCurrentWordIndex(){return mCurrentWordIndex;};
	int						GetWrongLetterCount(){return mWrongLetterCounter;};
	void					GetBlockedDropletAreas(std::vector<IvAABB>& aabbs);
	bool					IsPlayingLostSequence(){return mPlayingLostSequence;};
	
	virtual std::string GetTypeString()
	{
		return "GameLogic";
	}
	
private:
	GameLogic &operator=(const GameLogic& m);
	GameLogic(const GameLogic&m);

protected:
	static GameLogic*	mInstance;
	void ReadWordList(std::string path);
	void ProcessChosenLetters();
	void StartNextWord();
};
#endif