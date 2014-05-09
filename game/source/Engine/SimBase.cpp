/*
 *  SimBase.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 12/25/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */

#ifdef TARGETIPHONE
#import <OpenGLES/ES1/glext.h>
#elif WIN32
#include <windows.h>
#include <GL/gl.h>

#else
#import <OpenGL/gl.h>
#import <OpenGL/OpenGL.h>
#endif
#include "SimBase.h"


SimBase::SimBase(){
	mPaused = true;
}

SimBase::~SimBase(){
	
}

void SimBase::Start(unsigned int time){
	mPaused = false;
}

void SimBase::Stop(){
	mPaused = true;
}

void SimBase::Pause(){
	mPaused = true;
}

void SimBase::Unpause(){
	mPaused = false;
}


//void SimBase::tick(unsigned int dt, unsigned int systime, unsigned int simtime){
//
//}

void SimBase::Update(){
	
}

void SimBase::FixedUpdate(){
	
}

void SimBase::LateUpdate(){
	
}

void SimBase::MouseDown(float u, float v, int button){
	
}

void SimBase::MouseDragged(float u, float v,float lu, float lv, int button){

}

void SimBase::MouseUp(float u, float v, int button){

}

//void SimBase::updateAccelerometer(float u, float v,float w){
//
//}

void SimBase::Render(){

}
