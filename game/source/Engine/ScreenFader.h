#ifndef _SCREEN_FADER_H
#define _SCREEN_FADER_H

#include "RenderObject.h"

class Game;
class GameLogic;
class LetterChooser;
class WordProgressDisplay;
class HangedManDisplay;
class Sequence;
class MetaballGrid;
class TextureFont;
class LetterDisplayAnimation;

class ScreenFader : public RenderObject
{
protected:
	unsigned int						mStartTime;
	unsigned int						mDuration;
	float								mZPosition;
	IvColor								mStartColor;
	IvColor								mEndColor;
	IvColor								mCurrentColor;

public:
	ScreenFader(){mDuration = 1000; mStartColor.Set(0,0,0,0); mEndColor.Set(0,0,0,1);};
	ScreenFader(PersistentData& storageData);
	~ScreenFader();

	void Update();
	void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StopAsSequenceItem();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	virtual std::string GetTypeString()
	{
		return "ScreenFader";
	}
private:
	ScreenFader &operator=(const GameLogic& m);
	ScreenFader(const GameLogic&m);

};
#endif //_SEQUENCE_DELAYER_H