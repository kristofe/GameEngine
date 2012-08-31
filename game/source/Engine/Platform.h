/*
 *  Platform.h
 *  GWEngine_1.0
 *
 *  Created by Kristofer Schlachter on 12/26/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */

#ifdef TARGET_OS_IPHONE
	#include "Platform_iPhone.h"
#elif TARGET_OS_MAC
	#include "Platform_Mac.h"
#else
	#include "Platform_Win32.h"
#endif
