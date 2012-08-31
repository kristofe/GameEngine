#ifndef _WORD_PROGRESS_H
#define _WORD_PROGRESS_H

#include "RenderObject.h"

class HangingLetterSign;
class TextureFont;
class WordProgressDisplay : public RenderObject
{
	TextureFont*						mFont;
	//std::string						mDebugString;
	std::string							mTargetWord;
	std::vector<HangingLetterSign*>		mDisplayTiles;
	//std::vector<IvVector3>			mNewLetterPositions;
	std::multimap<char,HangingLetterSign*>	mLetterSlots;
	int									mSpaceBetweenTiles;
	char								mLastChosenLetter;
	int									mRotationDuration;
	int									mWindDuration;
	std::string							mRenderTargetTag;
public:
	WordProgressDisplay(TextureFont* font);
	WordProgressDisplay(PersistentData& storageData);
	virtual ~WordProgressDisplay();

	//void SetDebugString(std::string s);
	void SetTargetWord(std::string s);
	void LetterChosen(char c);
	void BlowTilesAround();
	char GetLastChosenLetter(){return mLastChosenLetter;};
	virtual void MouseDown(float u, float v, int button){};
	virtual void MouseDragged(float u, float v,float lu, float lv, int button){};
	virtual void MouseUp(float u, float v, int button){};
	virtual void LevelLoaded();
	virtual void ProcessMessage(GameMessage& msg);
	virtual void GetLetterPositions(char c, std::vector<IvVector3>& positionList);

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	void ClearTiles();
	void ShowWholeWord(std::string word);
	
	virtual std::string GetTypeString()
	{
		return "WordProgressDisplay";
	}
	
	virtual std::string DebugOutput();	
protected:
	std::string ToCSV(std::string& s);
	void GenerateTiles();
	

};

#endif