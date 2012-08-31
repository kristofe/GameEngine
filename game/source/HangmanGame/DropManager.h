#ifndef _DROP_MANAGER_H
#define _DROP_MANAGER_H

#include "GameObject.h"

class Game;
class GameLogic;
class LetterChooser;
class WordProgressDisplay;
class HangedManDisplay;
class Sequence;
class MetaballGrid;
class MetaballManager2D;
class TextureFont;
class LetterDisplayAnimation;

struct Droplet{
	unsigned int mID;
	IvVector3	mPosition;
	IvVector3   mVelocity;
	float		mRadius;
	float		mPower;
	bool		mLeavesTrail;
	bool		mStutter;

	Droplet()
	{
		mID = 0;
		mRadius = 0;
		mPower = 0;
		mLeavesTrail = true;
		mStutter = false;
	}
	
	Droplet& operator=(const Droplet &d) {
		if(this == &d) return *this;
		
		mID = d.mID;
		mPosition = d.mPosition;
		mVelocity = d.mVelocity;
		mRadius = d.mRadius;
		mPower = d.mPower;
		mLeavesTrail = d.mLeavesTrail;
		mStutter = d.mStutter;
		
	}
	
	Droplet(const Droplet& d)
	{
		mID = d.mID;
		mPosition = d.mPosition;
		mVelocity = d.mVelocity;
		mRadius = d.mRadius;
		mPower = d.mPower;
		mLeavesTrail = d.mLeavesTrail;
		mStutter = d.mStutter;
	}
	
};

class DropManager : public GameObject
	{
	protected:
		GameLogic*							mGameLogic;
		std::list<Droplet*>					mActiveDroplets;
		std::string							mGameLogicTag;
		
		MetaballGrid*						mMetaballGrid;
		std::string							mMetaballGridTag;
		//MetaballManager2D*					mMetaballMgr2D;
		//std::string							mMetaballMgr2DTag;
		
		IvVector3							mEmitterPosition0;
		IvVector3							mEmitterPosition1;
		
		
		unsigned int						mLastUpdateTime;

		static DropManager*					mInstance;
		
	public:
		static DropManager& GetInstance();
		DropManager();
		DropManager(PersistentData& storageData);
		~DropManager();
		
		
		virtual void ProcessMessage(GameMessage& msg);
		
		void LateUpdate();
		void Animate();
		void Splash();
		void AddDrop(unsigned int creatorID, IvVector3& pos, IvVector3& vel, bool leavesTrail = true, bool stutter = false, float maxRadius = 0.055f, float maxPower = 0.75f);
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		void Cleanup();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		void RemoveDroplet(Droplet* d);
		void CreateDroplet(unsigned int creatorID, IvVector3& pos, IvVector3 velocity, float radius, float power, bool leavesTrail, bool stutter = false);
		
		virtual std::string GetTypeString()
		{
			return "DropManager";
		}
		
		virtual std::string DebugOutput()
		{
			char buf[256];
			sprintf(buf, "\tActiveDropletCount:%d",(int)mActiveDroplets.size());
			std::string superInfo = GameObject::DebugOutput();
			std::string s = superInfo + buf;
			return s;
		}
		
	private:
		DropManager &operator=(const DropManager& m);
		DropManager(const DropManager&m);
		
	};
#endif