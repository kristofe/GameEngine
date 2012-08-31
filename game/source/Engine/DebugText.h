#ifndef _DEBUG_TEXT_H
#define _DEBUG_TEXT_H

#include "TextureText.h"

class DebugText : public TextureText
{
	std::string		mDisplayText;
public:
	
	DebugText(TextureFont* font);
	DebugText(PersistentData& storageData);
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	void SetDisplayText(const char* s);
	void SetDisplayText(std::string s);
	std::string GetDisplayText();
	
	virtual std::string GetTypeString()
	{
		return "DebugText";
	}
};

#endif//_DEBUG_TEXT_H