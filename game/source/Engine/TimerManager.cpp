#include "Platform.h"
#include "TimerManager.h"

////////////////////////////////////////////////////////////////////////////////
Timer::Timer(TimerManager* mgr,void* userData,unsigned int currentTime, unsigned int duration, bool dieAtLevelEnd, TimerCallback finishCallback, TimerCallback tickCallback){
	mMgr				=	mgr;
	mStartTime		=	currentTime;
	mCurrentTime		=	currentTime;
	mDuration			=	duration;
	mExpired			=	false;
	mPercentDone		=	0.0f;
	mFinishCallback	=	finishCallback;
	mTickCallback		=	tickCallback;
	mUserData			=	userData;
	mDieAtLevelEnd		=	dieAtLevelEnd;
}
////////////////////////////////////////////////////////////////////////////////	
Timer::~Timer(){
	
}
////////////////////////////////////////////////////////////////////////////////
void Timer::Update(unsigned int currentTime){
	mCurrentTime = currentTime;
	unsigned int elapsed = mCurrentTime - mStartTime;
	if(elapsed < mDuration){
		mPercentDone = elapsed/(float)mDuration;
		if(mTickCallback){
			(*mTickCallback)(this);
		}
	}else{
		mExpired = true;
		mPercentDone = 1.0f;
		if(mFinishCallback){
			mFinishCallback(this);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void Timer::Reset(unsigned int duration){
	if(duration >= 0){
		mDuration		=	duration;
	}
	mStartTime	=	mCurrentTime;
	mExpired		=	false;
	mPercentDone	=	0.0f;
}
////////////////////////////////////////////////////////////////////////////////
TimerManager::TimerManager(Game* pGame){
	mCurrentTime		= GetTicks();
	
	
}
////////////////////////////////////////////////////////////////////////////////
TimerManager::~TimerManager(){
	std::list< Timer* >::iterator it = mTimerList.begin();
	while(it != mTimerList.end()){
		delete (*it);
		++it;
	}	
}
////////////////////////////////////////////////////////////////////////////////
void TimerManager::Update(unsigned int currentTime){
	mCurrentTime = currentTime;
	mKilledTimersList.clear();
	std::list< Timer* >::iterator it = mTimerList.begin();
	while(it != mTimerList.end()){
		(*it)->Update(currentTime);
		if((*it)->IsExpired()){
			mKilledTimersList.push_back((*it));
		}
		++it;
	}
	std::list< Timer* >::iterator it2 = mKilledTimersList.begin();
	while(it2 != mKilledTimersList.end()){
		DeleteTimer(*it2);
		++it2;
	}
}
////////////////////////////////////////////////////////////////////////////////
void TimerManager::CreateTimer(void* userData,unsigned int duration,TimerCallback finishCallback, bool dieAtLevelEnd,TimerCallback tickCallback){
	Timer* t = new Timer(this, userData, mCurrentTime,duration,dieAtLevelEnd, finishCallback,tickCallback);
	mTimerList.push_back(t);
}
////////////////////////////////////////////////////////////////////////////////
void TimerManager::DeleteTimer(Timer* timer){
	std::list< Timer* >::iterator it = mTimerList.begin();
	while(it != mTimerList.end()){
		if((*it) == timer){
			mTimerList.erase(it);
			break;
		}
		++it;
	}
	delete timer;
}
////////////////////////////////////////////////////////////////////////////////
void TimerManager::CleanupForLevelEnd(){
	std::list< Timer* > killedTimers;
	std::list< Timer* >::iterator it = mTimerList.begin();
	while(it != mTimerList.end()){
		if((*it)->GetDieAtLevelEnd() ){
			killedTimers.push_back(*it);
		}
		++it;
	}
	
	it = killedTimers.begin();
	while(it != killedTimers.end()){
		DeleteTimer(*it);
		++it;
	}
	
}
////////////////////////////////////////////////////////////////////////////////