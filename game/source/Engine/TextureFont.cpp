#include "TextureFont.h"
#include "Material.h"
#include "Platform.h"
#include "RendererObjectFactory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

TextureFont::TextureFont(std::string name)
{
	mName = name;
	Init();
}

void TextureFont::Init()
{
	std::string filename;
	filename.reserve(512);
	/*
	filename = name;
	filename += ".png";

	textureID = RendererObjectFactory::CreateTexture(name,filename.c_str());
	
	materialID = RendererObjectFactory::CreateMaterial(name);
	Material& mat = RendererObjectFactory::GetMaterial(materialID);
	mat.AddTexture(textureID);
	mat.mMainTexture = textureID;
	mat.mEmissive.Set(0.5f,0.5f,0.5f,1.0f);
	mat.isFont = true;

	filename = "";
	*/
	std::string resourcePath;
	GetResourcePath(resourcePath);
	filename += resourcePath;

	filename += "/";
	filename += mName;
	filename +=".fnt";
	FILE* fp = fopen(filename.c_str(), "r");

	if(!fp)
	{
		printf("Can't open font file for read!\n<%s>\n", filename.c_str());
		exit(-1);
	}

	char line[1024];
	
	// info line
	
	if (!fgets(line, 1024, fp))
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	
	// common line
	
	int w, h, pages;

	if (!fgets(line, 1024, fp))
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	
	if (sscanf(line, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d",
				 &lineHeight, &base, &w, &h, &pages) != 5)
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	
	// page line

	if (!fgets(line, 1024, fp))
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	int id;
	if (sscanf(line, "page id=%d file=\"%s\"",
				 &id, mFileName) != 2)
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	//get rid of the trailing "
	int idx = (int)strlen(mFileName);
	mFileName[idx-1] = 0;

	// chars line
	
	if (!fgets(line, 1024, fp))
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	
	int charCount;
	
	if (sscanf(line, "chars count=%d\n", &charCount) != 1)
	{
		fclose(fp);
		printf("Invalid font file format!\n");
		exit(-1);
	}
	
	// char lines
	
	for (int i = 0; i < 256; i++)
	{
		chars[i].available = false;
	}		
	
	for (int i = 0; i < charCount; i++)
	{
		if (!fgets(line, 1024, fp))
		{
			fclose(fp);
			printf("Invalid font file format!\n");
			exit(-1);
		}
		
		int c, temp;
		
		if (sscanf(line, "char id=%d", &c) != 1)
		{
			fclose(fp);
			printf("Invalid font file format!\n");
			exit(-1);
		}
		
		if (c > 255) continue;
		
		int ret = sscanf(line, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d",
				 &temp,
				 &(chars[c].x),
				 &(chars[c].y),
				 &(chars[c].w),
				 &(chars[c].h),
				 &(chars[c].xo),
				 &(chars[c].yo),
				 &(chars[c].xadv),
				 &(chars[c].page),
				 &(chars[c].chnl));

		if (ret == 10)
		{
			chars[c].available = true;
			/*
			printf("%d %d %d %d %d %d %d %d %d\n",
			   chars[c].x,
			   chars[c].y,
			   chars[c].w,
			   chars[c].h,
			   chars[c].xo,
			   chars[c].yo,
			   chars[c].xadv,
			   chars[c].page,
			   chars[c].chnl);
			 */
		}
		else
		{
			fclose(fp);
			printf("Invalid font file format!\n");
			exit(-1);
		}
		 
	}
	
	fclose(fp);
	
	for (int i = 0; i < 256; i++)
	{
		if (!chars[i].available) continue;
		chars[i].u1 = (float)chars[i].x/(float)w;
		chars[i].v1 = (float)chars[i].y/(float)h;
		chars[i].u2 = (float)(chars[i].x + chars[i].w)/(float)w;
		chars[i].v2 = (float)(chars[i].y + chars[i].h)/(float)h;
	}
}
////////////////////////////////////////////////////////////////////////////////
void TextureFont::Serialize(PersistentData& storageData)
{	
	storageData.SetName("TextureFont");
	storageData.SetProperty("Name",mName.c_str());
	storageData.SetProperty("TextureID",textureID);
	storageData.SetProperty("MaterialID",materialID);
	storageData.SetProperty("GameID",mGameID);
	
}
////////////////////////////////////////////////////////////////////////////////
void TextureFont::DeSerialize(PersistentData& storageData)
{
	mName = storageData.GetProperty("Name");
	textureID = static_cast<GameID> (storageData.GetPropertyU("TextureID"));
	materialID = static_cast<GameID> (storageData.GetPropertyU("MaterialID"));
	mGameID = static_cast<GameID> (storageData.GetPropertyU("GameID"));
	
	Init();
	
	RendererObjectFactory::RegisterTextureFont(mGameID, mName.c_str());
}
////////////////////////////////////////////////////////////////////////////////