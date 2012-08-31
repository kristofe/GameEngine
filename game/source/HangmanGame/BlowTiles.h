#ifndef _BLOW_TILES_H
#define _BLOW_TILES_H

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

class BlowTiles : public GameObject
	{
	protected:
		//GameLogic*							mGameLogic;
		//std::string							mGameLogicTag;
		
		unsigned int						mStartTime;
		unsigned int						mDuration;
		
	public:
		BlowTiles(){mDuration = 1000;};
		BlowTiles(PersistentData& storageData);
		~BlowTiles();
		
		void Update();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "BlowTiles";
		}
	private:
		BlowTiles &operator=(const GameLogic& m);
		BlowTiles(const GameLogic&m);
		
	};
#endif //_SEQUENCE_DELAYER_H