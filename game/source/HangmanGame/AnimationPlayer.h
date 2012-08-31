#ifndef _ANIMATION_PLAYER_H
#define _ANIMATION_PLAYER_H

#include "RenderObject.h"

class AnimatedTexture;
class RenderPlane;
class GameLogic;

class AnimationPlayer : public RenderObject
{
	protected: 
		AnimatedTexture*			mAnimation;
		RenderPlane*				mBackground;
		RenderPlane*				mForeground;

		IvVector3					mAnimationPosition;

		std::string					mGameLogicTag;
		GameLogic*					mGameLogic;

	public:
		AnimationPlayer(PersistentData& storageData);
		virtual ~AnimationPlayer();
		
		void ProcessMessage(GameMessage& msg);
		void LevelLoaded();
		void Update();//Override LetterDisplay::Update();

		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();

		virtual std::string GetTypeString()
		{
			return "AnimationPlayer";
		}
		
	virtual std::string DebugOutput();
};

#endif