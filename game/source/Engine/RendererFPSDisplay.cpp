#include "RendererFPSDisplay.h"
#include "Platform.h"
#include "Game.h"

////////////////////////////////////////////////////////////////////////////////
RendererFPSDisplay::RendererFPSDisplay(TextureFont* texFont)
						:TextureText(texFont)
{
	mLastUpdateTime = GetTicks();
	mUpdateIntervalInMillis = 2000;
	//SetString("FPS:  ");
	
	
}
////////////////////////////////////////////////////////////////////////////////
RendererFPSDisplay::RendererFPSDisplay(PersistentData& storageData)
:TextureText(storageData)
{
	mLastUpdateTime = GetTicks();
	mUpdateIntervalInMillis = storageData.GetPropertyI("UpdateInterval");
	SetString("FPS:  ");
	
}
////////////////////////////////////////////////////////////////////////////////
void RendererFPSDisplay::Serialize(PersistentData& storageData)
{
	TextureText::Serialize(storageData);
	storageData.SetName("RendererFPSDisplay");
	mLastUpdateTime = GetTicks();
	mUpdateIntervalInMillis = 2000;
}
////////////////////////////////////////////////////////////////////////////////
void RendererFPSDisplay::DeSerialize(PersistentData& storageData)
{
	TextureText::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void RendererFPSDisplay::Update()
{
	
        int t = GetTicks();
        if (t - mLastUpdateTime >= mUpdateIntervalInMillis )
        {
			int fps = (int)mGame.GetRenderer().GetFPS();
			char fpsStr[32];
			sprintf(fpsStr,"%sFPS: %d", Game::mMemoryState.c_str(),fps);
			
			SetString(fpsStr);
            mLastUpdateTime = t;
        }
    

}
////////////////////////////////////////////////////////////////////////////////