/*
 *  Thread.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 12/28/08.
 *  Copyright 2008 Gotham Wave Games, Inc. All rights reserved.
 *
 */
#ifndef _KDS_THREAD_H
#define _KDS_THREAD_H

#ifdef WIN32

#include "SDL_mutex.h"
#include "SDL_Thread.h"

typedef SDL_mutex Mutex;
typedef SDL_Thread Thread;


class Threading
{
public:
	static Mutex* MutexInit()
	{
		Mutex* m = SDL_CreateMutex();
		return m;
	};
	static void MutexLock(Mutex* m)
	{
		SDL_mutexP(m);
	};
	static void MutexUnlock(Mutex* m)
	{
		SDL_mutexV(m);
	};
	static void MutexDestroy(Mutex* m)
	{
		SDL_DestroyMutex(m);
	};
	static Thread* ThreadCreate(int (*fn)(void *), void *data)
	{
		Thread *t= SDL_CreateThread(fn,data);
		return t;
	};
	static void ThreadDestroy(Thread* t)
	{
		SDL_KillThread(t);
	};
	static int ThreadWait(Thread* t)
	{
		int status;
		SDL_WaitThread(t,&status);
		return status;
	};
	
};

#else

#include <pthread.h>
typedef pthread_mutex_t Mutex;
typedef pthread_t Thread;

static Mutex m;
class Threading
{
public:
	static Mutex* MutexInit()
	{
		//Mutex m;
		//pthread_mutex_init(&m,NULL);
		return &m;
	};
	static void MutexLock(Mutex* m)
	{
		//pthread_mutex_lock(m);
	};
	static void MutexUnlock(Mutex* m)
	{
		//pthread_mutex_unlock(m);
	};
	static void MutexDestroy(Mutex* m)
	{
		//pthread_mutex_destroy(m);
	};
	static Thread* ThreadCreate(void* (*fn)(void *), void *data)
	{
		Thread *t;
		//pthread_create(t,NULL,fn,data);
		return t;
	};
	static void ThreadDestroy(Thread* t)
	{
		//pthread_exit(*t);
	};
	static void ThreadWait(Thread* t)
	{
		//pthread_join(*t);
	};
	
};

#endif

#endif //_KDS_THREAD_H