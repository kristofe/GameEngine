#include "DebugText.h"
#include "Platform.h"
#include "Game.h"

////////////////////////////////////////////////////////////////////////////////
DebugText::DebugText(TextureFont* texFont)
:TextureText(texFont)
{
	mDisplayText = "";
	//SetString("FPS:  ");
	
	
}
////////////////////////////////////////////////////////////////////////////////
DebugText::DebugText(PersistentData& storageData)
:TextureText(storageData)
{
	mDisplayText = "";
	SetString(mDisplayText.c_str());
	
}
////////////////////////////////////////////////////////////////////////////////
void DebugText::Serialize(PersistentData& storageData)
{
	TextureText::Serialize(storageData);
	storageData.SetName("DebugText");
	
}
////////////////////////////////////////////////////////////////////////////////
void DebugText::DeSerialize(PersistentData& storageData)
{
	TextureText::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void DebugText::SetDisplayText(const char* s)
{
	mDisplayText = s;
	SetString(mDisplayText.c_str());
}
////////////////////////////////////////////////////////////////////////////////
void DebugText::SetDisplayText(std::string s)
{
	mDisplayText = s;
	SetString(mDisplayText.c_str());
}
////////////////////////////////////////////////////////////////////////////////
std::string DebugText::GetDisplayText()
{
	return mDisplayText;
}
////////////////////////////////////////////////////////////////////////////////