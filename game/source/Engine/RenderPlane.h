#ifndef _RENDER_PLANE_H
#define _RENDER_PLANE_H

#include "RenderObject.h"


class RenderPlane : public RenderObject{
protected:
	bool					mDelayLoadingTexture;
	std::string				mTexturePath;
	GameID				mTextureID;
public:
	
	RenderPlane(const char* imagename);
	RenderPlane(GameID materialID);
	RenderPlane(PersistentData& storageData);
	~RenderPlane();

	void PreSetup(PersistentData& storageData);
	void SetupMeshes(PersistentData& storageData);
	void SetupMaterialsAndTextures(PersistentData& storageData);
	void PostSetup(PersistentData& storageData);
	void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StopAsSequenceItem();
	void SequenceOver(bool deleteObject = true);
	void LoadTexture();
	void UnloadTexture();
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	
private:
	//Remove Copy Constructor and assignment operator
	RenderPlane		&operator =  (const RenderPlane &m) { printf("RenderPlane = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	RenderPlane(const RenderPlane &m){printf("RenderPlane COPY CTOR SHOULDN'T BE GETTING CALLED");};;
	void Init(const std::string& imagename);
	
	virtual std::string GetTypeString()
	{
		return "RenderPlane";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "\tTextureID:%d\tTexturePath:%s\tDelayLoadingTexture:%s",(int)mTextureID,mTexturePath.c_str(),mDelayLoadingTexture?"true":"false");
		std::string superInfo = RenderObject::DebugOutput();
		std::string s = superInfo + buf;
		return s;
	}
	
};

#endif //_RENDER_PLANE_H