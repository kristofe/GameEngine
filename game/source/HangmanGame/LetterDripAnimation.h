#ifndef _LETTER_DRIP_ANIMATION_H
#define _LETTER_DRIP_ANIMATION_H

#include "GameObject.h"

class Game;
class GameLogic;
class LetterChooser;
class WordProgressDisplay;
class HangedManDisplay;
class Sequence;
class MetaballGrid;
class MetaballManager2D;
class TextureFont;
class LetterDisplayAnimation;

class LetterDripAnimation : public GameObject
{
protected:
	GameLogic*							mGameLogic;
	std::list<LetterDisplayAnimation*>	mLetterAnims;
	std::string							mGameLogicTag;

	MetaballGrid*						mMetaballGrid;
	std::string							mMetaballGridTag;
	MetaballManager2D*					mMetaballMgr2D;
	std::string							mMetaballMgr2DTag;
	TextureFont*						mFont;

	std::vector<IvVector3>				mStartPositions;
	std::vector<IvVector3>				mEndPositions;
	unsigned int						mDuration;
	char								mChar;

public:
	LetterDripAnimation();
	LetterDripAnimation(PersistentData& storageData);
	~LetterDripAnimation();


	virtual void ProcessMessage(GameMessage& msg);

	void Update();
	void Animate();
	void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StopAsSequenceItem();

	void Cleanup();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	virtual std::string GetTypeString()
	{
		return "LetterDripAnimation";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "\tLetterAnimCount:%d",(int)mLetterAnims.size());
		std::string superInfo = GameObject::DebugOutput();
		std::string s = superInfo + buf;
		return s;
	}
	

private:
	LetterDripAnimation &operator=(const GameLogic& m);
	LetterDripAnimation(const GameLogic&m);

};
#endif