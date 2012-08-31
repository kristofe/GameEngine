#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

#include "GameObject.h"
#include <list>


class TimerManager;
/*
 class TimerListener{
 public:
 
 virtual void TimerEvent(int event_id) = 0;
 };
 */
class Timer;

#ifdef WIN32
typedef  void (__cdecl *TimerCallback)(Timer*);//This declares a pointer to function that returns void and takes in Timer* as parameter.  The new Type is TimerCallback
#else
typedef  void (*TimerCallback)(Timer*);//This declares a pointer to function that returns void and takes in Timer* as parameter.  The new Type is TimerCallback
#endif

class Timer{
private:
	Timer(Timer& go);
	Timer& operator= (const Timer& go);
	
protected:
	TimerManager*	mMgr;
	unsigned int	mStartTime;
	unsigned int	mCurrentTime;
	unsigned int	mDuration;
	float			mPercentDone;
	bool			mExpired;
	void*			mUserData;
	TimerCallback	mFinishCallback;			
	TimerCallback	mTickCallback;
	bool			mDieAtLevelEnd;
	
public:
	Timer(TimerManager* mgr,void* userData,unsigned int currentTime, unsigned int duration, bool dieAtLevelEnd, TimerCallback finishCallback, TimerCallback tickCallback = NULL);
	virtual ~Timer();
	//virtual void Start();
	virtual void Reset(unsigned int duration = -1);
	virtual void Update(unsigned int currentTime);
	virtual float GetPercentDone(){return mPercentDone;};
	inline bool IsExpired(){ return mExpired;};
	inline bool GetDieAtLevelEnd(){ return mDieAtLevelEnd;};
	inline void SetTickCallback(TimerCallback pFunc){mTickCallback = pFunc;};
	inline void SetFinishCallback(TimerCallback pFunc){mFinishCallback = pFunc;};
	void* GetUserData(){return mUserData;};
	inline TimerManager& GetManager(){return *mMgr;};
	//virtual void SetCallbackObject(TimerListener* p){pCallbackObject_ = p;};
};

class TimerManager : GameObject{
protected:
	std::list< Timer* >		mTimerList;
	std::list< Timer* >		mKilledTimersList;
	unsigned int			mCurrentTime;
	
private:
	//Hide default constructor, copy constructor and assignment operator
	//LevelManager(){};
	TimerManager(TimerManager& go);
	TimerManager& operator= (const TimerManager& go);
	
	
public:
	TimerManager(){};
	TimerManager(Game* pGame);
	virtual ~TimerManager();
	
	void Update(unsigned int currentTime);
	void CreateTimer(void* userData,unsigned int duration,TimerCallback finishCallback, bool dieAtLevelEnd = true,TimerCallback tickCallback = NULL);
	void DeleteTimer(Timer* t);
	void CleanupForLevelEnd();
};
#endif