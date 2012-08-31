#ifndef _ANIMATED_TEXURE_H
#define _ANIMATED_TEXURE_H

#include <string>
#include <vector>

#include "RenderObject.h"

struct AnimationData2D
{
	std::string			animationName;
	int					textureCount;//textureIDX = (currFrame%colCount*rowCount)%frames.size();
	bool				isPVR;
	int					colsPerTexture;
	int					rowsPerTexture;
	int					cellsPerTexture;
	int					startRow;
	int					startCol;
	int					totalCellsAcrossAllTextures;
	float				fps;
	float				duration;
	std::string			dopeSheet;
	std::vector<int>	frames;
    
	AnimationData2D();
	AnimationData2D(PersistentData& storageData);
	void Init();
	std::string ToString();
	AnimationData2D	&operator=(const AnimationData2D &m);
    void ReadDopeSheet();
	PersistentData& Serialize(PersistentData& parent);
	void DeSerialize(PersistentData& parent);
};

class AnimatedTexture : public RenderObject{
protected:
	bool					mPaused;
	bool					mLoop;
	bool					mPlayImmediately;
	bool					mHalfWay;
	unsigned int			mStartTime;
	int						mCurrentFrameIdx;
	int						mCurrentTextureIdx;
	std::vector<GameID>	mTextures;
	AnimationData2D			mAnimationData;
	std::string				mAnimationName;
	std::string				mBasePath;
	float					mDU;
	float					mDV;
	IvVector2*				mUV;
	GameID				mCurrentTextureID;
	bool					mDataLoaded;

public:
	AnimatedTexture(AnimationData2D ad);
	AnimatedTexture(PersistentData& storageData);
	~AnimatedTexture();
	
	void AddEventListener(std::string eventName, GameID listener);
	void Init(AnimationData2D& ad);
	void LoadData();
	void UnloadData();
	void Update();
	void Animate();
	void GotoAndPlay(int frameNumber);
	void Play(){mPaused = false;};
	void Stop(){mPaused = true;};
	void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StopAsSequenceItem();
	AnimationData2D& GetAnimationData(){return mAnimationData;};
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	std::string GetAnimationName(){return mAnimationName;};
	
	virtual std::string GetTypeString()
	{
		return "AnimatedTexture";
	}
private:
	//Remove Copy Constructor and assignment operator
	AnimatedTexture		&operator =  (const AnimatedTexture &m) { printf("AnimatedTexture = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	AnimatedTexture(const AnimatedTexture &m){printf("AnimatedTexture COPY CTOR SHOULDN'T BE GETTING CALLED");};;

	
	
};

#endif //_RENDER_PLANE_H