#ifndef _TEXTURE_FONT_H
#define _TEXTURE_FONT_H

#include "Texture.h"

struct TextureChar
{
	bool available;
	int x, y, w, h, xo, yo, xadv, page, chnl;
	float u1, v1, u2, v2;
	/*
	PersistentData& Serialize(PersistentData& parent)
	{
		PersistentData* c = new PersistentData("TextureChar");
		c->SetProperty("available",available);
		c->SetProperty("x",x);
		c->SetProperty("y",y);
		c->SetProperty("w",w);
		c->SetProperty("h",h);
		c->SetProperty("x0",x0);
		c->SetProperty("y0",y0);
		c->SetProperty("TotalCellsAcrossAllTextures",totalCellsAcrossAllTextures);
		c->SetProperty("FPS",fps);
		c->SetProperty("Duration",duration);
		c->SetProperty("DopeSheet",dopeSheet.c_str());
		parent.AddChild(c);
		return *c;
	}
	
	static void DeSerialize(PersistentData& parent,AnimationData2D& v)
	{
		std::string n = "TextureChar";
		PersistentData* c = parent.GetUniqueChild(n);
		if(c != NULL)
		{
			v.animationName = c->GetProperty("AnimationName");
			v.textureCount =  c->GetPropertyI("TextureCount");
			v.colsPerTexture = c->GetPropertyI("ColsPerTexture");
			v.rowsPerTexture = c->GetPropertyI("RowsPerTexture");
			v.cellsPerTexture = c->GetPropertyI("CellPerTexture");
			v.startRow = c->GetPropertyI("StartRow");
			v.startCol = c->GetPropertyI("StartCol");
			v.totalCellsAcrossAllTextures = c->GetPropertyI("TotalCellsAcrossAllTextures");
			v.fps = c->GetPropertyF("FPS");
			v.duration = c->GetPropertyF("Duration");
			v.dopeSheet = c->GetProperty("DopeSheet");
		}
		
	}
	 */
};

class TextureFont
{
	friend class Renderer;
	friend class RendererObjectFactory;
	friend class TextureText;
	friend class LetterManager;
	friend class LetterDisplay;
	friend class LetterDisplayAnimation;

	int lineHeight, base;
	std::string		mName;
	char			mFileName[128];
	TextureChar chars[256];
	GameID  textureID;//should be a GameID or a Texture or a Material
	GameID	materialID;
	GameID	mGameID;
public:
	TextureFont(std::string name);
	GameID GetID(){return mGameID;};
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	GameID GetMaterialID(){return materialID;};
	GameID GetTextureID(){return textureID;};
	GameID GetGameID(){return mGameID;};
	std::string GetName(){return mName;};
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "%d\tTextureFont\tName:%s\ttextureID:%d\tmaterialID:%d",mGameID,mName.c_str(),textureID,materialID);
		std::string s = buf;
		return s;
	}
private:
	void Init();
};

#endif