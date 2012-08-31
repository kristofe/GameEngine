#ifndef _GAME_ENGINE_TIME_H
#define _GAME_ENGINE_TIME_H


class Time{
public:
	static unsigned int		frameNumber;
	static unsigned int		computerTimeMillis;
	static float			computerTime;
	static float			time;
	static float			deltaTime;
	static float			timeScale;
    
	static void Start();
	static void Stop();
	static void Pause(bool pause);
    static void Update();
	static void Init();
	static void FixedUpdate(){};
private:
	static bool				running;
	static unsigned int		lastUpdateTime;
    
};

#endif //_GAME_ENGINE_TIME_H