#ifndef _HANGING_LETTER_SIGN_H
#define _HANGING_LETTER_SIGN_H

#include "LetterDisplay.h"

class HangingLetterSign : public LetterDisplay
{
	protected: 
		char						mPendingChar;
		unsigned int				mStartTime;
		unsigned int				mLastUpdateTime;
		unsigned int				mRotationDuration;
		unsigned int				mWindDuration;
		LetterDisplay*				mLetterDisplay;
	private:
		bool						mUseWindAnimation;
	public:
		HangingLetterSign(TextureFont* font, unsigned int rotationAnimationDuration = 2000, unsigned int windAnimationDuration = 3000);
		virtual ~HangingLetterSign();
		
		void Init();
		void SetChar(char c);
		void LateUpdate();
		void Update(){};//Override LetterDisplay::Update();
		void Animate();
		void IdleAnimate();
		void BlowAround();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();

		virtual const float	GetCameraDistance() const {return mPosition.z-20;/*Forcing this to drawn last*/ /*THIS IS NOT CORRECT... Need to multiply mPosition by inverse trans of camera!*/};
	

		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "HangingLetterSign";
		}
	
	protected:
		void AnimateWind();
		void AnimateReveal();
};

#endif