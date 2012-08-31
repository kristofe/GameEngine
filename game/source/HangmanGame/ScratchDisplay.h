#ifndef _SCRATCH_DISPLAY_H
#define _SCRATCH_DISPLAY_H

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



class ScratchDisplay : public GameObject
	{
	protected:
		GameLogic*							mGameLogic;
		std::vector<RenderPlane*>			mRenderPlanes;
		std::string							mGameLogicTag;
		RenderPlane*						mCurrentRenderPlane;
		unsigned int						mCurrentStateID;
	public:
		ScratchDisplay();
		ScratchDisplay(PersistentData& storageData);
		~ScratchDisplay();
		
		
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
			return "ScratchDisplay";
		}
		
		virtual std::string DebugOutput();		
	private:
		ScratchDisplay &operator=(const GameLogic& m);
		ScratchDisplay(const GameLogic&m);
		
	};
#endif