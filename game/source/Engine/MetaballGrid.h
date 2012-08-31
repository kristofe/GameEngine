#ifndef _METABALL_GRID_H
#define _METABALL_GRID_H


#include "FluidGrid.h"
#include "IvVector2.h"
#include "LetterDisplay.h"

struct MetaballTrackingData{
	GameID			mGameID;
	GameID			mCreatorID;
	unsigned int	mCreationTime;
	unsigned int	mLastUpdateTime;
	IvVector3		mLastPosition;
	IvVector3		mLastDripPosition;
	float			mLastDripRadius;
	float			mRadius;
	float			mPower;
	bool			mErasable;
	bool			mEraseOthers;
	bool			mLeavesTrail;
	
	MetaballTrackingData()
	{
		mErasable = true;
		mEraseOthers = true;
		mLeavesTrail = true;
		mGameID = mCreatorID = mCreationTime = mLastUpdateTime = 0;
		mLastDripRadius = mRadius = mPower = 0.0f;
	}
};

class MetaballGrid: public FluidGrid
{
protected:
	std::vector<unsigned short>	mIndices;
	
public:
	MetaballGrid(int nx, int ny, float width, float height, std::string imagename);
	MetaballGrid(PersistentData& storageData);
	~MetaballGrid();
	
	void Init(int nx, int ny, float width, float height, std::string imagename);
	void EffectField(float x, float y, float radius, float power = 1.0f, bool staticField = false);
	void Update();
	void setUVs(float * vals);
	/*
	void MouseDown(float u, float v, int button);
	void MouseDragged(float u, float v,float lu, float lv, int button);
	void MouseUp(float u, float v, int button);
	*/
	
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	void AddObject(unsigned int id, unsigned int creatorID, IvVector3& pos, float radius = LETTER_METABALL_RADIUS, float power = 1.0f, bool leavesTrail = true, bool eraseable = false, bool erasesOthers = true);
	void RemoveObject(unsigned int id);
	void ObjectMoved(unsigned int id, IvVector3& newPos, IvVector3& vel = IvVector3::origin);
	void MakePendingRemovals();
	void ClearGridValues();
	void GetIntersectingDrops(std::list<MetaballTrackingData>& dropList, MetaballTrackingData& td);
	
	virtual std::string GetTypeString()
	{
		return "MetaballGrid";
	}
private:
	float*	mMetaballFieldValues;
	//float*	mStaticMetaballFieldValues;
	float*	mMetaballGradientValues;
	//float*	mZeroArray;
	float   mWindowWidth;
	float	mWindowHeight;
	bool	mRenderUsingIsoSurface;
	std::vector<IvVector2>	mOriginalUVs;
	
	std::map<unsigned int, MetaballTrackingData>	mGameObjectDB;
	std::list<unsigned int>							mPendingDeletions;

protected:
	void EffectStaticField(float x, float y, float radius, float power = 1.0f);
	void UpdateDynamicMetaballGrid(IvVector2& pos, float radiusSqr, float radius,  float sign,float power);
	void GenerateIsoSurface( );
	void ConstructMeshFromLocalData();
	void ExpandMesh();
	
	inline void CheckEdge(float threshold, IvVector2& a_pos,IvVector2& b_pos,float a_val, float b_val, float *out_x, float *out_y)
	{
		IvVector2 diff,vertex;
		float	ratio;
		
		diff = a_pos - b_pos;
		ratio = (threshold - a_val) / (b_val - a_val);
		diff *= ratio;
		vertex = a_pos - diff;
		
		
		
		*out_x = vertex.x;
		*out_y = vertex.y;
	};
	
};

#endif