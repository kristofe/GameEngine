//
//  MainView.m
//  GLGravity
//
//  Created by Kristofer Schlachter on 5/9/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//
#import <OpenGLES/ES1/gl.h>
#import <UIKit/UIKit.h>
#import "MainView.h"

#include "Platform.h"
#include "Game.h"
#include "Scene.h"
#include "RendererGLES11.h"


// CONSTANTS
#define kAccelerometerFrequency		30.0 //Hz
#define kUpdateFrequency			1.0/120.0 //Hz
#define kFilteringFactor			0.2

/* screen width, height, and bit depth */
#define WINDOW_WIDTH  320
#define WINDOW_HEIGHT 480
#define WINDOW_BPP     32


@implementation MainView


- (id)initWithFrame:(CGRect)frame {
	if (self = [super initWithFrame:frame]) {
		// Initialization code
	}
	
	self.multipleTouchEnabled = YES;
	
	//Create the OpenGL ES view and add it to the window
	_glView = [[EAGLView alloc] initWithFrame:frame pixelFormat:GL_RGBA8_OES depthFormat:GL_DEPTH_COMPONENT16_OES preserveBackbuffer:NO];
	[self addSubview:_glView];
	
	
	//Configure and start accelerometer
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	
	_renderTimer = [NSTimer 
					scheduledTimerWithTimeInterval:kUpdateFrequency
					target: self 
					selector: @selector(update) 
					userInfo: nil
					repeats: YES
					];

	[_glView setCurrentContext];
	
	if([_glView isOpenGLES20])
	{
		
	}else
	{
		_renderer = new RendererGLES11(WINDOW_WIDTH, WINDOW_HEIGHT, [_glView getFrameBuffer],[_glView getRenderBuffer]);
    }
	_gameEngine = &(Game::GetInstance());
	_gameEngine->SetRenderer(_renderer);
	_renderer->SetGameEngine(_gameEngine);
	_gameEngine->LoadXMLData("Config.xml");
	_gameEngine->Start(0);
	
	
	mStartTime = GetTimeInMilliSeconds();
	mLastUpdateTime = mStartTime;
	mDeltaTime = 0;
		
	[self reshape ];
	
	//Threading the game!
	//[NSThread detachNewThreadSelector: @selector( gameLoopForThread ) toTarget:self withObject:nil ];
	
	return self;
}



- (void)dealloc {
	[_glView release];
	if( _renderTimer != nil && [_renderTimer isValid ] ){
		[ _renderTimer invalidate ];
		_renderTimer = nil;
	}
	delete _gameEngine;
	delete _renderer;
	[super dealloc];
}

/*
- (void) ClearRenderer{
	[_glView setCurrentContext];
    // Drawing code here.
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 0.f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}
 */

- (void) gameLoopForThread{	
	NSAutoreleasePool*	pool = [ NSAutoreleasePool new ];
	while(true){
		[_glView setCurrentContext];
		_gameEngine->Update();
		_gameEngine->Render();
		_renderer->Render();
		_gameEngine->LateUpdate();
		[_glView swapBuffers];
		//[NSThread sleepForTimeInterval:0.001];
	}
	[pool release];	
}

- (void) update{	
	[_glView setCurrentContext];
	_gameEngine->Update();
	_gameEngine->Render();
	_renderer->Render();
	_gameEngine->LateUpdate();
	[_glView swapBuffers];
}
	

- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration {
	//Use a basic low-pass filter to only keep the gravity in the accelerometer values
	_accelerometer.x = acceleration.x * kFilteringFactor + _accelerometer.x * (1.0 - kFilteringFactor);
	_accelerometer.y = acceleration.y * kFilteringFactor + _accelerometer.y * (1.0 - kFilteringFactor);
	_accelerometer.z = acceleration.z * kFilteringFactor + _accelerometer.z * (1.0 - kFilteringFactor);
	
	
	
		
		
		
		float xx = fabs(_accelerometer.x);
		if(xx < 0.025f) xx = 0.025f;
		float angleForX = atan2(_accelerometer.y, xx);
		float angleForY = atan2(_accelerometer.z, -_accelerometer.x);

		
		
		_orientation.z = _accelerometer.x;
		_orientation.x = angleForX;
		_orientation.y = angleForY;

		
		_gameEngine->UpdateAccelerometerAndOrientation(_accelerometer,_orientation);
		

}

/*
 
 //////////////////////////////////////////////////////////////////////////////////////////////////// 
 private const float AccelerometerUpdateInterval = 1.0f / 60.0f;
 private const float LowPassKernelWidthInSeconds = 0.1f;
 private const float LowPassFilterFactor = AccelerometerUpdateInterval / LowPassKernelWidthInSeconds; // tweakable
 private Vector3 lowPassValue = Vector3.zero; // should be initialized with 1st sample
 ////////////////////////////////////////////////////////////////////////////////////////////////////   
 Vector3 LowPassFilter(Vector3 newSample) {
 lowPassValue = Vector3.Lerp(lowPassValue, newSample, LowPassFilterFactor);
 return lowPassValue;
 }
 
 if(iPhoneInput.accelerationEventCount > 0 &&  iPhoneInput.accelerationEventCount <= 5){
 Vector3 acceleration = Vector3.zero;
 
 int eventCount = iPhoneInput.accelerationEventCount;
 if(eventCount > 5){
 Debug.Log("iPhoneInput.accelerationEventCount > 10 " + iPhoneInput.accelerationEventCount);
 eventCount = 5;
 }
 float period = 0.0f;
 for (int i = 0; i < eventCount; ++i) {
 iPhoneAccelerationEvent accEvent  = iPhoneInput.GetAccelerationEvent(i);
 acceleration += accEvent.acceleration * accEvent.deltaTime;
 period += accEvent.deltaTime;
 LowPassFilter(accEvent.acceleration * accEvent.deltaTime);
 }
 if(period > 0)
 {
 acceleration *= 1.0f/period;	
 }
 
 accelerator = lowPassValue;
 lastAccelerator = iPhoneInput.acceleration;   
 
 
 
 float xx = Mathf.Abs(accelerator.x);
 if(xx < 0.025f) xx = 0.025f;
 float angleForX = Mathf.Atan2(accelerator.y, xx);
 float angleForY = Mathf.Atan2(accelerator.z, -accelerator.x);
 //float angleRadians = angle * 180.0f/3.14159f;
 
 
 accelerator.z = accelerator.x/(Mathf.PI);
 accelerator.x = -angleForX/(Mathf.PI);
 accelerator.y = -angleForY/(Mathf.PI);
 
 //float maxAcc = 0.1f;
 accelerator *= 0.55f;
 //accelerator.x = Mathf.Clamp(accelerator.x,-maxAcc,maxAcc);
 //accelerator.y = Mathf.Clamp(accelerator.y,-maxAcc,maxAcc);
 //accelerator.z = Mathf.Clamp(accelerator.z,-maxAcc,maxAcc);
 
 
 
 }
 }
 
 */
 
/////////////////////////////////////////////////////////////////////////////////////////////////
- (void) reshape
{ 	
	CGSize size = [_glView surfaceSize];
	mFrameWidth = size.width;
	mFrameHeight = size.height;

	//_renderer->SetWindowSize(mFrameWidth, mFrameHeight);
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
	//Like a mouse down
	UITouch       *touch = [touches anyObject];
	if([touch tapCount] == 2) {
		//
//		VortexEngine* ve = (VortexEngine*)_gameEngine->getSim();
//		if(ve)
//			ve->swapSimulations();
		return;
	}
	
	NSArray* allTouches = touches.allObjects;
	for(unsigned int i = 0; i < [allTouches count]; i++){
		UITouch *touch = [allTouches objectAtIndex:i];
		CGPoint currentTouchPosition = [touch locationInView:self];
		
		_gameEngine->MouseDown(currentTouchPosition.x/((float)mFrameWidth),1.0 - (currentTouchPosition.y/((float)mFrameHeight)), i);
	}
	
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
	//Like a mousedragged
	NSArray* allTouches = touches.allObjects;
	for(unsigned int i = 0; i < [allTouches count]; i++){
		UITouch *touch = [allTouches objectAtIndex:i];
		CGPoint currentTouchPosition = [touch locationInView:self];
		CGPoint lastTouchPosition = [touch previousLocationInView:self];
		
		_gameEngine->MouseDragged(currentTouchPosition.x/((float)mFrameWidth),1.0 - (currentTouchPosition.y/((float)mFrameHeight)),lastTouchPosition.x/((float)mFrameWidth),1.0 - (lastTouchPosition.y/((float)mFrameHeight)), i);
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
	//Like a mouse up
	NSArray* allTouches = touches.allObjects;
	for(unsigned int i = 0; i < [allTouches count]; i++){
		UITouch *touch = [allTouches objectAtIndex:i];
		CGPoint currentTouchPosition = [touch locationInView:self];
		
		_gameEngine->MouseUp(currentTouchPosition.x/((float)mFrameWidth),1.0 - (currentTouchPosition.y/((float)mFrameHeight)), i);
	}
	
}

-(EAGLView*)getView{
	return _glView;
}

- (void)didReceiveMemoryWarning {
	_gameEngine->MemoryWarning();
	
	//[super didReceiveMemoryWarning];
}

@end



