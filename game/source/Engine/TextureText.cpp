#include "TextureText.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
////////////////////////////////////////////////////////////////////////////////
TextureText::TextureText(TextureFont* texFont):mFont(texFont)
{
	Init();
}
////////////////////////////////////////////////////////////////////////////////
TextureText::TextureText(PersistentData& storageData):RenderObject(storageData)
{
	//SetupFont
	std::string fontName = storageData.GetProperty("Font");
	GameID fontID = RendererObjectFactory::CreateTextureFont(fontName.c_str());
	TextureFont& font = RendererObjectFactory::GetTextureFont(fontID);
	mFont = &font;
	Init();
	width = storageData.GetPropertyF("width");
}
////////////////////////////////////////////////////////////////////////////////
void TextureText::Init()
{
	//width = 320;
	align = valign = 0;
	width = height = 0;
	text[0] = NULL;
	std::string meshName;
	meshName = "TextureText";
	char buff[64];
	sprintf(buff,"%03d",mID);
	meshName += buff;
	GameID meshID = RendererObjectFactory::CreateMesh(meshName.c_str());
	mMeshes.push_back(meshID);//has to be added to this list  - in order to be released;
	mMaterials.push_back(mFont->materialID);//has to be added to this list  - in order to be released;

	mGame.GetRenderer().AddRenderObject(mID);//This is where it is specified that it is text
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,mFont->materialID);
	mGame.GetRenderer().Retain(mID,mFont->textureID);
	
}
////////////////////////////////////////////////////////////////////////////////
TextureText::~TextureText()
{
	mFont = NULL;
	for(int i = 0; i < (int)mMeshes.size(); ++i)
	{
		mGame.GetRenderer().Release(mID,mMeshes[i]);
		Material& m = RendererObjectFactory::GetMaterial(mMaterials[i]);
		for(int j = 0; j < m.mTextureCount; j++)
		{
			mGame.GetRenderer().Release(mID,m.mTextureIDs[j]);
		}
		mGame.GetRenderer().Release(mID,mMaterials[i]);
	}
	//SAFE_DELETE(text);
}

////////////////////////////////////////////////////////////////////////////////
void TextureText::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("TextureText");
	
	
	char buff[128];
	unsigned int objID = static_cast<unsigned int>(mFont->GetID());
	sprintf(buff, "%d",objID);
	storageData.SetProperty("Font",objID);
	
	storageData.SetProperty("Width",width);
	storageData.SetProperty("Height",height);
	storageData.SetProperty("Align",align);
	storageData.SetProperty("VAlign",valign);
	PersistentData* textNode = new PersistentData("Text");
	std::string txt = text;
	textNode->SetTextProperty(txt);
	storageData.AddChild(textNode);
	

	
}
////////////////////////////////////////////////////////////////////////////////
void TextureText::DeSerialize(PersistentData& storageData)
{
	SceneObject::DeSerialize(storageData);
	
	GameID fontID = static_cast<GameID> (storageData.GetPropertyU("Font"));
	TextureFont& font = Game::GetInstance().GetRenderer().GetTextureFont(fontID);
	mFont = &font;

	width = storageData.GetPropertyF("Width");
	height = storageData.GetPropertyF("Height");
	
	align = storageData.GetPropertyI("Align");
	valign = storageData.GetPropertyI("VAlign");
	
	PersistentData* textNode = storageData.GetNodeFromPath("Text");
	std::string s = textNode->GetText();
	if(s.size() < TEX_TEXT_MAX_STRING_LENGTH)
	{
		printf("TextureText::DeSerialize() ERROR:DESERIALIZED STRING THAT WAS TOO LONG:%s",s.c_str());
		SetString("ERROR DESERIALIZING - String too long");
	}
	SetString(s.c_str());
	
}
////////////////////////////////////////////////////////////////////////////////

void TextureText::SetString(const char* str)
{
	strcpy(text, str);
	//strcpy(text,TEX_TEXT_MAX_STRING_LENGTH,str);
	GenerateLetters();
	CreateMesh();

}
////////////////////////////////////////////////////////////////////////////////
void TextureText::GenerateLetters()
{	
	//BitmapData* bitmapData = font->bitmapData;
	
	//if (bitmapData == NULL) return;
	int wordStart = 0;
	int wordWidth = 0;
	words.clear();
	
	bool whitespace = text[0] == ' ';
	
	for (int i = 0; ; i++)
	{
		//printf("%d (%d): ", i, text[i]);
		
		bool endWord = text[i] == ' ';
		bool newline = text[i] == '\n';
		if (whitespace && !newline) endWord = !endWord;
		if (text[i] == '\0' || newline) endWord = true;
		if (i == 0) endWord = false;
		
		if (endWord)
		{
			TextWord word;
			word.width = wordWidth;
			word.start = wordStart;
			word.end = i;
			word.whitespace = whitespace;
			word.newline = false;
			words.push_back(word);
			
			//printf("word: s=%d e=%d w=%d\n ", word.start, word.end, word.width);
			
			wordStart = i;
			wordWidth = 0;
			whitespace = !whitespace;
		}

		if (text[i] == '\0') break;
		if (text[i] == '\n'){
			TextWord word;
			word.width = 1;
			word.start = i;
			word.end = i;
			word.whitespace = false;
			word.newline = true;
			words.push_back(word);
		}
		
		TextureChar* c = &(mFont->chars[text[i]]);
		if (!c->available) continue;
		wordWidth += c->xadv;
	}
	
	TextWord word;
	word.width = 0;
	word.start = 0;
	word.end = 0;
	word.whitespace = true;
	words.push_back(word);
		
	//printf("Got Words!\n");
	
	int lineStart = 0;
	int lineEnd = 0;
	int lineWidth = 0;
	int whitespaceWidth = 0;
	int wordCount = 0;
	lines.clear();
	
	for (int i = 0; i < (int)words.size(); i++)
	{
		TextWord* word = &words[i];
		
		if (word->newline)
		{
			TextLine line;
			line.width = lineWidth;
			line.start = lineStart;
			line.end = lineEnd;
			line.wordCount = wordCount;
			line.terminal = false;
			lines.push_back(line);
//			printf("line: s=%d e=%d w=%d wc=%d\n ", line.start, line.end, line.width, line.wordCount);
			lineStart = i;
			lineWidth = 0;
			whitespaceWidth = 0;
			wordCount = 0;
			continue;
		}
		if (word->whitespace)
		{
			whitespaceWidth += word->width;
			continue;
		}
		
		bool newLine = lineWidth + whitespaceWidth + word->width > width;
//		if (i == 0) newLine = false;
//		if (lineWidth == 0 && word->width > width) newLine = false;
		
		if (newLine)
		{
			TextLine line;
			line.width = lineWidth;
			line.start = lineStart;
			line.end = lineEnd;
			line.wordCount = wordCount;
			line.terminal = false;
			lines.push_back(line);
//			printf("line: s=%d e=%d w=%d wc=%d\n ", line.start, line.end, line.width, line.wordCount);
			lineStart = i;
			lineWidth = 0;
			whitespaceWidth = 0;
			wordCount = 0;
		}
		lineWidth += whitespaceWidth;
		lineWidth += word->width;
		whitespaceWidth = 0;
		lineEnd = i + 1;
		wordCount++;
//		printf("WC: %d\n", wordCount);
	}
	
	TextLine line;
	line.width = lineWidth;
	line.start = lineStart;
	line.end = lineEnd;
	line.terminal = true;
	lines.push_back(line);
	//printf("line: s=%d e=%d w=%d\n ", line.start, line.end, line.width);

	//printf("Got Lines!\n");
	
	float px = 0;
	float py = 0;
	
	if (valign == VALIGN_BOTTOM) py += height - lines.size()*mFont->lineHeight;
	if (valign == VALIGN_MIDDLE) py += (height - lines.size()*mFont->lineHeight)/2.0f;
	
	for (int l = 0; l < (int)lines.size(); l++)
	{
		TextLine* line = &lines[l];
//		printf("Line %d: %d %d\n", l, line->end - line->start, line->wordCount);
		float wordSpacing = 0;
//		if (!line->terminal && line->end - line->start > 1)
		if (align == ALIGN_JUSTIFY && !line->terminal && line->wordCount > 1)
		{
//			wordSpacing = float(width - line->width)/float(line->end - line->start - 1);
			wordSpacing = float(width - line->width)/float(line->wordCount - 1);
		}
		
		bool isFirstWord = true;
		for (int w = line->start; w < line->end; w++)
		{
			TextWord* word = &words[w];
//			if (w != line->start) px += wordSpacing;
			if (word->whitespace)
			{
				px += word->width;
			}
			else
			{
				if (isFirstWord)
				{
					if (align == ALIGN_CENTER) px += float(width - line->width)/2.0f;
					else if  (align == ALIGN_RIGHT) px += float(width - line->width);
					isFirstWord = false;
				}
				else
				{
					px += wordSpacing;
				}
				mStartingGeometryIndexOfLastWord = 999999999;
				for (int i = word->start; i < word->end; i++)
				{
					//printf("%d (%d): ", i, text[i]);
					TextureChar* c = &(mFont->chars[text[i]]);
					if (!c->available) continue;
					float u1 = c->u1;
					float v1 = c->v1;
					float u2 = c->u2;
					float v2 = c->v2;
					//printf("u1=%f v1=%f u2=%f v2=%f\n", u1, v1, u2, v2);
					int x1 = (int)(px + c->xo);
					int y1 = (int)(py - c->yo);
					int x2 = (int)(x1 + c->w);
					int y2 = (int)(y1 - c->h);

					////renderer.rectangle(bitmapData, x1, -y1, x2, -y2, u1, v1, u2, v2, 1.0f, 1.0f, 1.0f, 1.0f);
					AddLetterToMesh(x1,y1,x2,y2,u1,v1,u2,v2);
					px += c->xadv;
				}
			}
		}
		px = 0;
		py -= mFont->lineHeight;
	}
}
////////////////////////////////////////////////////////////////////////////////
void TextureText::AddLetterToMesh(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
	int startIDX = mVertices.size();
	
	if(mStartingGeometryIndexOfLastWord > startIDX)
		mStartingGeometryIndexOfLastWord = startIDX;
	
	IvVector3 pos;
	pos.Set(x1,y1,0);
	mVertices.push_back(pos);
	pos.Set(x2,y1,0);
	mVertices.push_back(pos);
	pos.Set(x2,y2,0);
	mVertices.push_back(pos);
	pos.Set(x1,y2,0);
	mVertices.push_back(pos);

	IvColor c = mColor;//(1.0f,1.0f,1.0f,1.0f);
	mColors.push_back(c);
	mColors.push_back(c);
	mColors.push_back(c);
	mColors.push_back(c);

	IvVector2 uv;
	uv.Set(u1,v1);
	mUVs.push_back(uv);
	uv.Set(u2,v1);
	mUVs.push_back(uv);
	uv.Set(u2,v2);
	mUVs.push_back(uv);
	uv.Set(u1,v2);
	mUVs.push_back(uv);

	mIndices.push_back(startIDX + 0);
	mIndices.push_back(startIDX + 2);
	mIndices.push_back(startIDX + 3);

	mIndices.push_back(startIDX + 0);
	mIndices.push_back(startIDX + 1);
	mIndices.push_back(startIDX + 2);

}
////////////////////////////////////////////////////////////////////////////////
void TextureText::CreateMesh()
{
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	m.Clear();
	m.mVertexCount = mVertices.size();
	//m.mNormalCount = mNormal;
	m.mColorCount = mColors.size();
	m.mUVCount = mUVs.size();
	m.mTriangleCount = mIndices.size()/3;

	m.mVertices = new IvVector3[m.mVertexCount];
	m.mNormals = new IvVector3[m.mNormalCount];
	m.mColors = new IvColor[m.mColorCount];
	m.mUV = new IvVector2[m.mUVCount];

	m.mTriangles = new unsigned short[mIndices.size()];

	for(int i = 0; i < (int)mVertices.size(); i++)
	{
		m.mVertices[i] = mVertices[i];
	}


	for(int i = 0; i < (int)mColors.size(); i++)
	{
		m.mColors[i] = mColors[i];
	}

	for(int i = 0; i < (int)mUVs.size(); i++)
	{
		m.mUV[i] = mUVs[i];
	}	

	for(int i = 0; i < (int)mIndices.size(); i++)
	{
		m.mTriangles[i] = mIndices[i];
	}
	mVertices.clear();
	mColors.clear();
	mUVs.clear();
	mIndices.clear();
	m.Unlock();

}
////////////////////////////////////////////////////////////////////////////////