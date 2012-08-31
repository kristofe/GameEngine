#ifndef _BACKGROUND_CHANGER_H
#define _BACKGROUND_CHANGER_H

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

class BackgroundChanger : public GameObject
	{
	protected:
		unsigned int						mBackgroundIndex;
		
	public:
		BackgroundChanger(){mBackgroundIndex = 0; };
		BackgroundChanger(PersistentData& storageData);
		~BackgroundChanger();
		
		void Update();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "BackgroundChanger";
		}
		
	private:
		BackgroundChanger &operator=(const GameLogic& m);
		BackgroundChanger(const GameLogic&m);
		
	};
#endif //_SEQUENCE_DELAYER_H
