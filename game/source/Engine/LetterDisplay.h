#ifndef _LETTER_DISPLAY_H
#define _LETTER_DISPLAY_H

#include "RenderObject.h"
#include "TextureFont.h"
#include <vector>
#include "IvAABB.h"

#define LETTER_METABALL_RADIUS  0.09f
#define LETTER_METABALL_RADIUS_FOR_SPRITE 64.0f

class MetaballGrid;
class MetaballManager2D;

class LetterDisplay : public RenderObject
	{
	protected: 
		char						mChar;
		std::vector<IvVector3>		mVertices;
		std::vector<IvVector2>		mUVs;
		std::vector<IvColor>		mColors;
		std::vector<unsigned short>	mIndices;
		bool						mDirtyMesh;
		MetaballGrid*				mMetaballGrid;
		std::string					mMetaballGridTag;
		MetaballManager2D*			mMetaballMgr2D;
		std::string					mMetaballMgr2DTag;
		TextureChar*				mTextureChar;
	public:
		TextureFont* mFont;

		LetterDisplay(TextureFont* font);
		LetterDisplay(PersistentData& storageData);
		virtual ~LetterDisplay();
		virtual void Init();
		void ConstructMeshFromLocalData();
		void UpdateMeshFromLocalData();
		void Update();
		virtual void SetChar(char c, bool center=true, IvColor& color = IvColor::white);
		virtual void LevelLoaded();
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		TextureChar* GetTextureChar(){return mTextureChar;};
		char GetChar(){return mChar;};

		virtual std::string GetTypeString()
		{
			return "LetterDisplay";
		}
		
		
		virtual std::string DebugOutput()
		{
			char buf[256];
			std::string fontName = "NONE";
			int fontID = 0;
			if(mFont != NULL)
			{
				fontName = mFont->GetName();
				fontID = (int)mFont->GetID();
			}
			sprintf(buf, "\tFontID:%d\tFontName:%s",fontID,fontName.c_str());
			std::string superInfo = RenderObject::DebugOutput();
			std::string s = superInfo + buf;
			return s;
		}
	};

#endif