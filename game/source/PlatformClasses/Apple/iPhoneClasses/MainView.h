//
//  MainView.h
//  GLGravity
//
//  Created by Kristofer Schlachter on 5/9/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//
//#import <OpenGLES/ES1/gl.h>
#import <UIKit/UIKit.h>
#import "EAGLView.h"
#include "IvVector3.h"

class Game;
class Renderer;



@interface MainView : UIView<UIAccelerometerDelegate> {
	EAGLView*				_glView;  // A view for OpenGL ES rendering
	IvVector3				_orientation;
	IvVector3				_accelerometer;
	NSTimer*				_renderTimer;
	
	Renderer*			_renderer;
	Game*				_gameEngine;
		
    int      mButtonDown;
    
    float    mMouseX;
    float    mMouseY;
    float    mMouseDownX;
    float    mMouseDownY;
	
	int		mFrameWidth;
	int		mFrameHeight;
	unsigned int mLastUpdateTime;
	unsigned int mStartTime;
	unsigned int mDeltaTime;
	unsigned int mCurrentTime;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void) gameLoopForThread;
- (void) update;
- (void) reshape;
- (EAGLView*) getView;
- (void)didReceiveMemoryWarning;

@end
