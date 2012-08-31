#ifndef _TEXTURE_TEXT_H
#define _TEXTURE_TEXT_H

#include "RenderObject.h"
#include "TextureFont.h"
#include <vector>

#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2
#define ALIGN_JUSTIFY 3

#define VALIGN_TOP 0
#define VALIGN_BOTTOM 1
#define VALIGN_MIDDLE 2

#define TEX_TEXT_MAX_STRING_LENGTH 4096
struct TextWord
{
	int start, end, width;
	bool whitespace;
	bool newline;
};

struct TextLine
{
	int start, end, width, wordCount;
	bool terminal;
};

class TextureText : public RenderObject
{
protected:
	std::vector<TextWord> words;
	std::vector<TextLine> lines;

	std::vector<IvVector3>		mVertices;
	std::vector<IvVector2>		mUVs;
	std::vector<IvColor>		mColors;
	std::vector<unsigned short>	mIndices;
	unsigned int				mStartingGeometryIndexOfLastWord;
public:
	TextureFont* mFont;
	float width, height;
	int align, valign;
	char text[TEX_TEXT_MAX_STRING_LENGTH];
	TextureText(TextureFont* font);
	TextureText(PersistentData& storageData);
	virtual ~TextureText();
	void Init();
	virtual void SetString(const char* str);
	void GenerateLetters();
	void AddLetterToMesh(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2);
	void CreateMesh();
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	virtual std::string GetTypeString()
	{
		return "TextureText";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "\tmFont:%s\twordcount:%d\tlinecount:%d",mFont->GetName().c_str(),(int)words.size(),(int)lines.size());
		std::string superInfo = RenderObject::DebugOutput();
		std::string s = superInfo + buf;
		return s;
	}
	
};

#endif