#ifndef _LETTER_MANAGER_H
#define _LETTER_MANAGER_H

#include "LetterDisplay.h"
#include "TextureFont.h"
#include <vector>
#include "IvAABB.h"



#define TOUCH_RADIUS 12
class MetaballGrid;

struct LetterButton
{
	unsigned int		mID;
	IvVector3			mPosition;
	IvAABB				mAABB;
	IvVector3			mCenter;
	float				mRadius;
	int					mVertices[4];
	int					mUVs[4];
	int					mColors[4];
	unsigned short		mIndices[6];
	char				mChar;
	TextureChar*		mTextureChar;
	MetaballGrid*		mMetaballGrid;
	
	
	LetterButton(unsigned int id, IvVector3& p, float r);
	~LetterButton();
	void RemoveFromMetaballGrid();
	void AddToMetaballGrid(MetaballGrid* m);
	
	bool HitTest(float x, float y)
	{
		IvVector3 min;
		min.Set(x,y,0);
		IvAABB p(min,min);
		return mAABB.Intersect(p);
	};
	
	IvVector3 CalcCenter()
	{
		IvVector3 min = mAABB.GetMinima();
		IvVector3 max = mAABB.GetMaxima();
		IvVector3 diff = max-min;
		float dist = Distance(max,min);
		diff.Normalize();
		mCenter= min + (diff*(dist*0.5f));
		return mCenter;
	};
	
	const IvVector3& GetPosition()
	{
		return mPosition;
	};

	void SetPosition(const IvVector3& p) {mPosition = p;};
	
	float GetRadius(){ return mRadius;};
	void SetRadius(float r){mRadius = r;};
	const IvVector3& GetCenter(){ return mCenter;};

};



class LetterManager : public LetterDisplay
	{
	protected:
		std::vector<LetterButton*> mSourceLetters;
		std::vector<LetterButton*> mActiveLetters;
		
		std::string					mLettersCSV; 
		LetterButton*				mSelectedLetter;
		/*
		std::vector<IvVector3>		mVertices;
		std::vector<IvVector2>		mUVs;
		std::vector<IvColor>		mColors;
		std::vector<unsigned short>	mIndices;
		
		bool						mDirtyMesh;
		MetaballGrid*				mMetaballGrid;
		std::string					mMetaballGridTag;
		*/
	public:
		//TextureFont* mFont;

		LetterManager(TextureFont* font);
		LetterManager(PersistentData& storageData);
		~LetterManager();
		virtual void Init();
		virtual void GenerateLetters(std::string lettersCSV);
		LetterButton* GenerateLetter(char targetLetter, int x, int y);
		void FillInGeometryData(LetterButton* letter, float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2);
		//void ConstructMeshFromLocalData();
		//void UpdateMeshFromLocalData();
		void Update();
		void AdjustMeshForActiveLetters();
		virtual void MouseDown(float u, float v, int button);
		virtual void MouseDragged(float u, float v,float lu,float lv, int button);
		virtual void MouseUp(float u, float v, int button);
		//virtual void LevelLoaded();
		void SetLetterColor(LetterButton* letter, IvColor& c);
		void MoveLetter(LetterButton* letter, int dx, int dy);
		void SetLetterPosition(LetterButton* letter, int x, int y);
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "LetterManager";
		}
	};

#endif