#ifndef _HANGED_MAN_DISPLAY
#define _HANGED_MAN_DISPLAY

#include "TextureText.h"

class HangedManDisplay : public TextureText
{
	std::string		mDisplayText;
public:

	HangedManDisplay(TextureFont* font);
	HangedManDisplay(PersistentData& storageData);
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	void SetDisplayText(const char* s);
	void SetDisplayText(std::string s);
	std::string GetDisplayText();
	
	virtual std::string GetTypeString()
	{
		return "hangedManDisplay";
	}
};

#endif