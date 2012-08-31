#ifndef _RENDERER_FPS_DISPLAY
#define _RENDERER_FPS_DISPLAY

#include "TextureText.h"

class RendererFPSDisplay : public TextureText
{
	int	mLastUpdateTime;
	int mUpdateIntervalInMillis;
public:

	RendererFPSDisplay(TextureFont* font);
	RendererFPSDisplay(PersistentData& storageData);
	void Update();
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	virtual std::string GetTypeString()
	{
		return "RendererFPSDisplay";
	}
	
};

#endif