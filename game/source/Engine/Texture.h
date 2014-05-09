/*
 *  Texture.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/7/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TEXTURE_H
#define _TEXTURE_H

//#include "GameObject.h"
#include <string>
#include "Renderer.h"
#include "RendererObjectFactory.h"

class Texture{
	friend class RendererObjectFactory;
	friend class Renderer;
	friend class RendererGL;
public:
	std::string			mSourceFileName;
	std::string			mShortFileName;
	std::string			mName;
	unsigned int*		mPixelData;
	unsigned int		mDataLength;
	unsigned int		mWidth;
	unsigned int		mHeight;
	unsigned int		mBitsPerPixel;
	unsigned int		mHardwareID;
	unsigned int		mNumberOfColors;
	unsigned int		mPixelFormat; //GL_RGB, GL_RGBA, 
	bool				mIsCompressed;
	bool				mHasAlpha;
	GameID				mGameID;
	bool				mKeepLocalPixelData;
	bool				mDelayLoad;
	
public:
	Texture(std::string name,std::string filename,bool delayLoad = false);
	~Texture();
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	GameID GetID(){return mGameID;};
	void OnDataUploaded();
	void UnloadTextureData();
	void LoadTextureData();

	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "%d\tTexture\tName:%s\tmHardwareID:%d\tKeepPixelData:%s\tPixelPointerAddress:%08x\tDelayLoad:%s",mGameID,mName.c_str(),mHardwareID,mKeepLocalPixelData?"true":"false",(unsigned int*)mPixelData,mDelayLoad?"true":"false");
		std::string s = buf;
		return s;
	}

private:
	//Remove Copy Constructor and assignment operator
	Texture		&operator =  (const Texture &t) { printf("Texture = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	Texture(const Texture &t){printf("Texture COPY CTOR SHOULDN'T BE GETTING CALLED");};
	

};


#endif //_TEXTURE_H