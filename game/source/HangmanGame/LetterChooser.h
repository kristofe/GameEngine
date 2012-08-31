#ifndef _LETTER_CHOOSER_H
#define _LETTER_CHOOSER_H

#include "LetterManager.h"

class GameLogic;

class LetterChooserMessage  : public GameMessage
{
public:
	LetterButton*	mLetterButton;

};
class LetterChooser : public LetterManager
{
protected:
	enum LayoutScheme 
	{
		GRID,
		SINE,
		RANDOM
	};


	IvVector2				mDisplayMin;
	IvVector2				mDisplayMax;
	LayoutScheme			mLayoutScheme;
	float					mJitterStrength;
	int						mRows;
	LetterButton*			mLastLetterChosen;
	bool					mNextClickActivates;
	//bool					mLockInput;
	std::vector<IvVector3>	mPositionList;
	std::map<char,IvVector3> mPositionDB;
	
	GameLogic*							mGameLogic;
	std::string							mGameLogicTag;

public:
	LetterChooser(TextureFont* font,IvVector2& min, IvVector2& max, LayoutScheme layoutScheme, float jitterPower);
	LetterChooser(PersistentData& storageData);
	virtual ~LetterChooser();

	virtual void MouseUp(float u, float v, int button);
	virtual void Reset();
	virtual void AdjustLayout();
	virtual void Clear();
	virtual void LevelLoaded();
	
	//void LockInput(){mLockInput = true;};
	//void UnlockInput(){mLockInput = false;};
	LetterButton* GetLastChosenLetter(){return mLastLetterChosen;};
	
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	void NextClickActivates(){mNextClickActivates = true;};
	void GetLetterPositions(std::vector<IvVector3>& positions);
	IvVector3 GetLetterPosition(char c);
	void GetChars(std::vector<char>& chars);
	void CalculatePositions();

	virtual std::string GetTypeString()
	{
		return "LetterChooser";
	}
	
protected:
	void PlaceLetters();
	void GridLayout();
	void SineLayout(){};
	void RandomLayout();
	const char* MethodToString();

};

#endif