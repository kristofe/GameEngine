#ifndef _FLIP_LETTER_H
#define _FLIP_LETTER_H

#include "GameObject.h"

class Game;
class GameLogic;


class FlipLetter : public GameObject
{
protected:
	GameLogic*							mGameLogic;
	std::string							mGameLogicTag;


public:
	FlipLetter();
	FlipLetter(PersistentData& storageData);
	~FlipLetter();

	void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StopAsSequenceItem();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	virtual std::string GetTypeString()
	{
		return "FlipLetter";
	}
private:
	FlipLetter &operator=(const GameLogic& m);
	FlipLetter(const GameLogic&m);

};
#endif