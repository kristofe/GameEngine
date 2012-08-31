#ifndef _POEM_DISPLAY
#define _POEM_DISPLAY

#include "TextureText.h"

class PoemDisplay : public TextureText
	{
		std::string					mDisplayText;
		std::vector<std::string>	mLines;
		std::vector<std::string>	mWords;
		std::string					mBGObjectTag;
		RenderObject*				mBGObject;
		unsigned int				mStartTime;
		unsigned int				mDuration;
		unsigned int				mAnimationDuration;
		int							mLastIdxShown;
		int							mCurrentWordIndex;
		bool						mFinished;
	public:
		
		PoemDisplay(TextureFont* font);
		PoemDisplay(PersistentData& storageData);
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		void MouseUp(float u, float v, int button);
		void Update();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		void SequenceOver(bool deleteObject = true);
		void SendFinishMessage();
		
		void SetDisplayText(const char* s);
		void SetDisplayText(std::string s);
		void ReadWordPoem(std::string filename);
		void DisplayUpToWord(int idx);
		void SetLastWordAlpha(const float a, unsigned int startIDx);
		
		int	GetWordCount(){return (int)mWords.size();};
		
		virtual std::string GetTypeString()
		{
			return "PoemDisplay";
		}
		
		virtual std::string DebugOutput()
		{
			char buf[256];
			unsigned int bgID = 0;
			if(mBGObject != NULL)
				bgID = (unsigned int)mBGObject->GetID();
			sprintf(buf, "\tBGObjectID:%d\tBGObjectTag:%s",(int)bgID,mBGObjectTag.c_str());
			std::string superInfo = TextureText::DebugOutput();
			std::string s = superInfo + buf;
			return s;
		}
};

#endif

