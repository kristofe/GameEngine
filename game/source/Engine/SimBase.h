/*
 *  SimBase.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 12/25/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */


#ifndef SIM_BASE_H
#define SIM_BASE_H

#include "Globals.h"
#include <stdio.h>

class SimBase{
public:
	SimBase();
	~SimBase();
	
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	//virtual void tick(unsigned int dt, unsigned int systime, unsigned int simtime);
	virtual void Render();
	virtual void MouseDown(float u, float v, int button);
	virtual void MouseDragged(float u, float v,float lu,float lv, int button);
	virtual void MouseUp(float u, float v, int button);
	//virtual void UpdateAccelerometer(float u, float v,float w);
	virtual void Start(unsigned int time = 0);
	virtual void Stop();
	virtual void Pause();
	virtual void Unpause();
	virtual void LevelLoaded(){};
	virtual void LevelStart(){};
	virtual void LevelEnd(){};
	
	virtual bool isPaused() { return mPaused;};
protected:
	bool		 mPaused;
	
private:
	//Remove Copy Constructor and assignment operator
	SimBase		&operator =  (const SimBase &t) { printf("SimBase = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	SimBase(const SimBase &t){printf("SimBase COPY CTOR SHOULDN'T BE GETTING CALLED");};
};

#endif //SIM_BASE_H