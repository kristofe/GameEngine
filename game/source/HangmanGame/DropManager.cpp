#include "DropManager.h"
#include "LetterDisplayAnimation.h"
#include "WordProgressDisplay.h"
#include "LetterChooser.h"
#include "Game.h"
#include "GameLogic.h"
#include "Platform.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "MetaballManager2D.h"
#include "Scene.h"
#include "Perlin.h"

////////////////////////////////////////////////////////////////////////////////
DropManager* DropManager::mInstance = NULL;
////////////////////////////////////////////////////////////////////////////////
DropManager& DropManager::GetInstance()
{
	
	return *mInstance;
	
}
////////////////////////////////////////////////////////////////////////////////
DropManager::DropManager(PersistentData& storageData):GameObject(storageData)
{
	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	mMetaballGridTag = storageData.GetProperty("TargetMetaballGrid");
	//mMetaballMgr2DTag = storageData.GetProperty("TargetMetaballManager2DTag");
	if(storageData.GetPropertyB("RefsAreImmediate"))
	{
		mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
		mMetaballGrid = static_cast<MetaballGrid*>(mGame.GetTaggedObject(mMetaballGridTag));
		//mMetaballMgr2D = static_cast<MetaballManager2D*>(mGame.GetTaggedObject(mMetaballMgr2DTag));
	}
	mInstance = this;
	
}
////////////////////////////////////////////////////////////////////////////////
DropManager::~DropManager()
{
	Cleanup();
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::RemoveDroplet(Droplet* d)
{
	if(mMetaballGrid != NULL)
		mMetaballGrid->RemoveObject(d->mID);
	
	std::list<Droplet*>::iterator it = mActiveDroplets.begin();
	while(it != mActiveDroplets.end())
	{
		if((*it)->mID == d->mID)
		{
			mActiveDroplets.erase(it);
			SAFE_DELETE(d);
			break;
		}
		++it;
	}
	
	
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::Serialize(PersistentData& storageData)
{
	GameObject::Serialize(storageData);
	storageData.SetName("DropManager");
	
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::DeSerialize(PersistentData& storageData)
{
	GameObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::LateUpdate()
{
	
	 if(!mActive)
		 return;
	
	std::list<Droplet*> removals;
	int dropCount = 0;
	unsigned int currTime = GetTicks();
	float elapsed = (currTime - mLastUpdateTime)*0.001f;
	
	std::list<Droplet*>::iterator it = mActiveDroplets.begin();
	while(it != mActiveDroplets.end())
	{
		Droplet* d = (*it);
		float noise = 1.0f;
		if(d->mStutter)
		{	
			noise = fabs(Perlin::Noise(d->mPosition.x*10.0f,d->mPosition.y*0.01f,currTime*0.004f));
			//float normNoise = 0.5f*noise + 0.5f;
		}
		
		
		if(d->mPosition.y > -50.0f)
		{
			d->mPosition += d->mVelocity*elapsed*(noise);
			//d->mPosition.y = roundf(d->mPosition.y);
			mMetaballGrid->ObjectMoved(d->mID, d->mPosition,d->mVelocity);
			dropCount++;
		}
		else
		{
			removals.push_back(d);
		}
		
		
		
		++it;
		
	}

	it = removals.begin();
	while(it != removals.end())
	{
		RemoveDroplet(*it);
		++it;
	}
	
	mLastUpdateTime = currTime;
	//if(dropCount == 0)
	//	mActive = false;
}

////////////////////////////////////////////////////////////////////////////////
void DropManager::Splash()
{
	mActive = true;
	mLastUpdateTime = GetTicks();
	
	srand(GetTicks());
	int dropCount = 6;
	for(int i = 0; i < dropCount; i++)
	{
		int x = 20 + (280/dropCount)*i + rand()%8;
		int y = rand() % 100;
		IvVector3 pos(x,400 - y,0);
		IvVector3 vel(0,-420.0f+ rand()%150,0);
		int shouldTrail = rand()%3;
		AddDrop(0,pos, vel,shouldTrail>=1?true:false, false);
	}
}

////////////////////////////////////////////////////////////////////////////////
void DropManager::AddDrop(unsigned int creatorID, IvVector3& pos, IvVector3& vel, bool leavesTrail, bool stutter,float maxRadius, float maxPower)
{
	//NOTE: Due to the current grid resolution(48x72) the minimum radius is 0.025 in order to get smooth 
	// interpolation across more than one grid cell during movement.
	// minimum power isn't as important but drops are visible as low as 0.1 (but only refractions effects)
	// max power should be around .75 higher than that an grid artifacts are very noticeable
	
	//Power 0.1 and redius 0.025 are very good for animated trails!
	

	float t = rand()/((float)RAND_MAX);
	//float radius = LERP(t,0.025f,0.055f);
	float radius = LERP(t,0.035f,maxRadius);
	t = rand()/((float)RAND_MAX);
	//float power = LERP(t,0.1f,0.75f);
	float power = LERP(t,0.1f,maxPower);
	CreateDroplet(creatorID,pos, vel, radius, power,leavesTrail,stutter);
	
}
////////////////////////////////////////////////////////////////////////////////
void  DropManager::CreateDroplet(unsigned int creator, IvVector3& pos, IvVector3 velocity, float radius, float power,bool leavesTrail, bool stutter)
{
	Droplet* d = new Droplet();
	d->mID = Game::GetNextGameID();
	if(creator == 0)
		creator = d->mID;
	d->mPosition = pos;
	d->mVelocity = velocity;
	d->mRadius = radius;
	d->mPower = power;
	d->mLeavesTrail = leavesTrail;
	d->mStutter = stutter;
	if(mMetaballGrid != NULL)
		mMetaballGrid->AddObject(d->mID, creator,d->mPosition, d->mRadius,d->mPower,d->mLeavesTrail,true,false);
	
	mActiveDroplets.push_back(d);
	
	
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::ProcessMessage(GameMessage& msg)
{

}

////////////////////////////////////////////////////////////////////////////////
void DropManager::StartAsSequenceItem(PersistentData* storageData)
{		
	/*
	 std::vector<IvVector3> endPositions;
	std::vector<IvVector3> startPositions;
	LetterChooser& lc = mGameLogic->GetLetterChooser();
	lc.CalculatePositions();
	lc.GetLetterPositions(endPositions);
	IvVector3 pos;
	pos.Set(160,-50,100);
	
	
	for(unsigned int i = 0; i < endPositions.size(); ++i)
	{
		startPositions.push_back(pos);
	}
	
	mStartPositions = startPositions;
	mEndPositions = endPositions;
	mActive = true;
	
	std::vector<char> chars;
	lc.GetChars(chars);
	for(unsigned int i = 0; i < mStartPositions.size(); ++i)
	{
		IvVector3 start = mStartPositions[i];
		IvVector3 end = mEndPositions[i];
		char c = chars[i];
		LetterDisplayAnimation* lda = new LetterDisplayAnimation(mFont,mMetaballGrid,mMetaballMgr2D,c,start, end,mDuration);
		mLetterAnims.push_back(lda);
		lda->AddEventListener("FINISHED",mID);
		lda->StartAsSequenceItem();
	}
	*/
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::StopAsSequenceItem()
{
	mActive = false;
	//mGame.GetActiveScene()->RemoveGameObject(this);
	Cleanup();
	
}
////////////////////////////////////////////////////////////////////////////////
void DropManager::Cleanup()
{

	mActiveDroplets.clear();
}
////////////////////////////////////////////////////////////////////////////////