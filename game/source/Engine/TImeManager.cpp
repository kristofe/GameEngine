
#include "Platform.h"
#include "TimeManager.h"

////////////////////////////////////////////////////////////////////////////////
//Static variables
unsigned int		Time::frameNumber;
unsigned int		Time::computerTimeMillis;
float				Time::time;
float				Time::computerTime;
float				Time::deltaTime;
float				Time::timeScale;
bool				Time::running;
unsigned int		Time::lastUpdateTime;
////////////////////////////////////////////////////////////////////////////////
void Time::Init()
{
	Time::frameNumber = 0;
	Time::timeScale = 1.0f;
}
////////////////////////////////////////////////////////////////////////////////
void Time::Update()
{	
	Time::frameNumber++;
	Time::computerTimeMillis = GetTicks();
	Time::computerTime = Time::computerTimeMillis * 0.001f;
	unsigned int dt = Time::computerTimeMillis - Time::lastUpdateTime; 
	Time::lastUpdateTime = Time::computerTimeMillis;
	
	Time::deltaTime = 0.0f;
	
	if(Time::running)
	{
		Time::deltaTime = dt*0.001f * Time::timeScale;
		Time::time += Time::deltaTime;
	}
	

}
////////////////////////////////////////////////////////////////////////////////

void Time::Start()
{ 
	Time::running = true; 
	 
	Time::lastUpdateTime = GetTicks();
}
////////////////////////////////////////////////////////////////////////////////

void Time::Stop()
{ 
	Time::running = false;
}
////////////////////////////////////////////////////////////////////////////////

void Time::Pause(bool pause)
{
	Time::running = pause;

}
////////////////////////////////////////////////////////////////////////////////
