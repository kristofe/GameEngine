/*
 *  Texture.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/7/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "Texture.h"
#include "Globals.h"
#include "Game.h"

////////////////////////////////////////////////////////////////////////////////
Texture::Texture(std::string name,std::string filename,bool delayLoad)
				:mName(name),
				mShortFileName(filename),
				mDelayLoad(delayLoad),
				mWidth(0),
				mHeight(0),
				mPixelData(NULL),
				mBitsPerPixel(0),
				mHardwareID(0),
				mPixelFormat(0x1908),//equal to GL_RGBA
				mIsCompressed(false),
				mHasAlpha(false),
				mGameID(0),
				mKeepLocalPixelData(false)
{
	

}
////////////////////////////////////////////////////////////////////////////////
Texture::~Texture()
{
	UnloadTextureData();
	SAFE_DELETE(mPixelData);

}
////////////////////////////////////////////////////////////////////////////////
void Texture::OnDataUploaded()
{
	if(!mKeepLocalPixelData)
		SAFE_DELETE(mPixelData);
	
}
////////////////////////////////////////////////////////////////////////////////

//TODO: This crap shouldn't be here
unsigned int		mWidth;
unsigned int		mHeight;
unsigned int		mBitsPerPixel;
unsigned int		mHardwareID;
unsigned int		mNumberOfColors;
bool				mHasAlpha;
bool				mIsLuminanceAlpha;


////////////////////////////////////////////////////////////////////////////////
void Texture::Serialize(PersistentData& storageData)
{
	storageData.SetName("Texture");
	storageData.SetProperty("SourceFileName",mSourceFileName.c_str());
	storageData.SetProperty("ShortFileName",mShortFileName.c_str());
	storageData.SetProperty("GameID",mGameID);
	storageData.SetProperty("Name", mName.c_str());
	
	
}
////////////////////////////////////////////////////////////////////////////////
void Texture::DeSerialize(PersistentData& storageData)
{
	mSourceFileName = storageData.GetProperty("SourceFileName");
	mShortFileName = storageData.GetProperty("ShortFileName");
	mGameID = static_cast<GameID> (storageData.GetPropertyU("GameID"));
	mName = storageData.GetProperty("Name");
	
	//Now I have to actually load the data!
	RendererObjectFactory::ReadTextureFromDisk(mShortFileName.c_str(),*this);
	//RendererObjectFactory::GetInstance().mTexturesDB[mName] = mGameID;
	RendererObjectFactory::RegisterTexture(mGameID, mName.c_str());
}
////////////////////////////////////////////////////////////////////////////////
void Texture::UnloadTextureData()
{
	if(mHardwareID != 0)
	{
		Game::GetInstance().GetRenderer().DeleteTextureFromGPU(*this);
		
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void Texture::LoadTextureData()
{
	if(!mShortFileName.empty() && mHardwareID == 0)// && mPixelData != NULL)
	{
		//printf("Reading %s from disk\n",mShortFileName.c_str());
		RendererObjectFactory::ReadTextureFromDisk(mShortFileName.c_str(),*this);
	}	
}
////////////////////////////////////////////////////////////////////////////////
