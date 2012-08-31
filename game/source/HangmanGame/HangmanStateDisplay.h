#ifndef _HANGMAN_STATE_DISPLAY_H
#define _HANGMAN_STATE_DISPLAY_H

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
class RenderPlane;
class IvAABB;

struct BlockerData{
	int state;
	std::vector<IvAABB> blockers;
};

class HangmanStateDisplay : public GameObject
{
protected:
	GameLogic*							mGameLogic;
	std::vector<RenderPlane*>			mRenderPlanes;
	std::string							mGameLogicTag;
	RenderPlane*						mCurrentRenderPlane;
	unsigned int						mCurrentStateID;
	std::vector<BlockerData*>			mBlockedAreasPerState;
public:
	HangmanStateDisplay();
	HangmanStateDisplay(PersistentData& storageData);
	~HangmanStateDisplay();


	void SetState(unsigned int stateID);
	//virtual void ProcessMessage(GameMessage& msg);
	//void Update();
	//void Animate();
	//void StartAsSequenceItem(PersistentData* storageData = NULL);
	//void StopAsSequenceItem();

	//void Cleanup();

	void LevelLoaded();
	
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	void GetBlockedDropletAreas(std::vector<IvAABB>& aabbs, int stateID);

	virtual std::string GetTypeString()
	{
		return "HangmanStateDisplay";
	}
	
	
	
private:
	HangmanStateDisplay &operator=(const GameLogic& m);
	HangmanStateDisplay(const GameLogic&m);

};
#endif