#ifndef _SPRAY_ANIMATION_H
#define _SPRAY_ANIMATION_H

#include "RenderObject.h"

class AnimatedTexture;
class RenderPlane;
class GameLogic;

class SprayAnimation : public RenderObject
	{
	protected: 
		AnimatedTexture*			mAnimation;
		IvVector3					mAnimationPosition;
		
		std::string					mGameLogicTag;
		GameLogic*					mGameLogic;
		
	public:
		SprayAnimation(PersistentData& storageData);
		virtual ~SprayAnimation();
		
		void ProcessMessage(GameMessage& msg);
		void LevelLoaded();
		void Update();//Override LetterDisplay::Update();
		
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		virtual std::string GetTypeString()
		{
			return "SprayAnimation";
		}
		
		virtual std::string DebugOutput();
	};

#endif