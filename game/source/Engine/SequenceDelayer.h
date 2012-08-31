#ifndef _SEQUENCE_DELAYER_H
#define _SEQUENCE_DELAYER_H

#include "GameObject.h"

class Game;
class GameLogic;
class LetterChooser;
class WordProgressDisplay;
class HangedManDisplay;
class Sequence;
class MetaballGrid;
class TextureFont;
class LetterDisplayAnimation;

class SequenceDelayer : public GameObject
{
protected:
	//GameLogic*							mGameLogic;
	//std::string							mGameLogicTag;

	unsigned int						mStartTime;
	unsigned int						mDuration;

public:
	SequenceDelayer(){mDuration = 1000;};
	SequenceDelayer(PersistentData& storageData);
	~SequenceDelayer();

	void Update();
	void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StopAsSequenceItem();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	virtual std::string GetTypeString()
	{
		return "SequenceDelayer";
	}
	
private:
	SequenceDelayer &operator=(const GameLogic& m);
	SequenceDelayer(const GameLogic&m);

};
#endif //_SEQUENCE_DELAYER_H