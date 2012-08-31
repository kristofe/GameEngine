//
//  main.m
//  GWEngine_1.0
//
//  Created by Kristofer Schlachter on 5/9/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = UIApplicationMain(argc, argv, nil,  @"AppController");
	[pool release];
	return retVal;
}
