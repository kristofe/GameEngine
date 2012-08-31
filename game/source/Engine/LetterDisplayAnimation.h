#ifndef _LETTER_DISPLAY_ANIMATION_H
#define _LETTER_DISPLAY_ANIMATION_H

#include "LetterDisplay.h"

class LetterDisplayAnimation : public LetterDisplay
{
	protected: 
		IvVector3					mStartPos;
		IvVector3					mEndPos;
		unsigned int				mStartTime;
		unsigned int				mDuration;
	public:
		LetterDisplayAnimation(TextureFont* font, MetaballGrid* metaballGrid,MetaballManager2D* metaballMgr2D, char c,IvVector3& start, IvVector3& end, unsigned int duration);
		virtual ~LetterDisplayAnimation();
		
		void Init();
		//void LateUpdate();
		void Update();//Override LetterDisplay::Update();
		void Animate();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();

		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
	
		virtual std::string GetTypeString()
		{
			return "LetterDisplayAnimation";
		}
	};

#endif