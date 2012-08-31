#include "LetterDisplay.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "MetaballManager2D.h"
////////////////////////////////////////////////////////////////////////////////
LetterDisplay::LetterDisplay(TextureFont* texFont):mFont(texFont)
{
	mMetaballGrid = NULL;
	mMetaballMgr2D = NULL;
	mTextureChar = NULL;
	mChar = '\0';
	mVisible = true;
	mActive = true;
	Init();
}
////////////////////////////////////////////////////////////////////////////////
LetterDisplay::LetterDisplay(PersistentData& storageData):RenderObject(storageData)
{
	mChar = '\0';
	mTextureChar = NULL;
	std::string fontName = storageData.GetProperty("Font");
	GameID fontID = RendererObjectFactory::CreateTextureFont(fontName.c_str());
	TextureFont& font = RendererObjectFactory::GetTextureFont(fontID);
	mFont = &font;
	mMetaballGrid = NULL;
	mMetaballMgr2D = NULL;
	mMetaballGridTag = storageData.GetProperty("TargetMetaballGridTag");
	mMetaballMgr2DTag = storageData.GetProperty("TargetMetaballManager2DTag");

	Init();
	
	std::string ch = storageData.GetProperty("Char");
	if(ch != "")
		mChar = *(ch.c_str());
	

}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::LevelLoaded()
{
	GameObject* go = mGame.GetTaggedObject(mMetaballGridTag);
	if(go)
	{
		mMetaballGrid = static_cast<MetaballGrid*>(go);
		//mMetaballGrid->AddObject(mID, mPosition);
	}
	
	go = mGame.GetTaggedObject(mMetaballMgr2DTag);
	if(go)
	{
		mMetaballMgr2D = static_cast<MetaballManager2D*>(go);
		
	}

	if(mChar != '\0')
	{
		SetChar(mChar);
	}
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::Init()
{
	//mSelectedLetter = NULL;
	std::string meshName;
	meshName = "LetterDisplay";
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
LetterDisplay::~LetterDisplay()
{
	mVertices.clear();
	mColors.clear();
	mUVs.clear();
	mIndices.clear();
	if(mMetaballGrid != NULL)
	{
		mMetaballGrid->RemoveObject(mID);
	}
}

////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("LetterDisplay");
	
	
	char buff[128];
	unsigned int objID = static_cast<unsigned int>(mFont->GetID());
	sprintf(buff, "%d",objID);
	storageData.SetProperty("Font",objID);
	/*
	 PersistentData* textNode = new PersistentData("Text");
	std::string txt = text;
	textNode->SetTextProperty(txt);
	storageData.AddChild(textNode);
	*/
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);
	
	GameID fontID = static_cast<GameID> (storageData.GetPropertyU("Font"));
	TextureFont& font = Game::GetInstance().GetRenderer().GetTextureFont(fontID);
	mFont = &font;
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::Update()
{
	if(mActive == false)
		return;
	
	if(mDirtyMesh)
	{
		UpdateMeshFromLocalData();
	}

	if(mMetaballGrid != NULL)
	{
		mMetaballGrid->EffectField(mPosition.x, mPosition.y, LETTER_METABALL_RADIUS);
	}
	
	if(mMetaballMgr2D != NULL)
	{
		mMetaballMgr2D->SetMetaballPosition(mPosition, LETTER_METABALL_RADIUS_FOR_SPRITE);
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::SetChar(char targetLetter, bool center, IvColor& c)
{	
	mVertices.clear();
	mColors.clear();
	mUVs.clear();
	mIndices.clear();
	ToUpper(&targetLetter);
	mChar = targetLetter;
	TextureChar* ch = &(mFont->chars[(int)(mChar)]);
	mTextureChar = ch;
	if (!ch->available) return;
	
	int startIDX = mVertices.size();

	float u1 = ch->u1;
	float v1 = ch->v1;
	float u2 = ch->u2;
	float v2 = ch->v2;
	/*int x1 = 0 + ch->xo;
	int y1 = 0 - ch->yo;
	int x2 = x1 + ch->w;
	int y2 = y1 - ch->h;*/
	int w = (int)(0.5f*ch->w);
	int h = (int)(0.5f*ch->h);
	int x1 = -w;
	int y1 = -ch->yo + 8;
	if(center)
	{
		x1 = - w;
		y1 =   h;
	}
	
	int x2 = x1 + ch->w;
	int y2 = y1 - ch->h;

	IvVector3 pos;
	pos.Set(x1,y1,0);
	mVertices.push_back(pos);
	pos.Set(x2,y1,0);
	mVertices.push_back(pos);
	pos.Set(x2,y2,0);
	
	mVertices.push_back(pos);
	pos.Set(x1,y2,0);
	mVertices.push_back(pos);
	
	//IvColor c(1.0f,1.0f,1.0f,1.0f);
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
	
	//int indiceStart = mIndices.size();
	mIndices.push_back(startIDX + 0);
	mIndices.push_back(startIDX + 2);
	mIndices.push_back(startIDX + 3);
	
	mIndices.push_back(startIDX + 0);
	mIndices.push_back(startIDX + 1);
	mIndices.push_back(startIDX + 2);
	
	ConstructMeshFromLocalData();
}
////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::ConstructMeshFromLocalData()
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
	m.Unlock();
	
	
}

////////////////////////////////////////////////////////////////////////////////
void LetterDisplay::UpdateMeshFromLocalData()
{

	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
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
	m.Unlock();
	mDirtyMesh = false;
	
}
////////////////////////////////////////////////////////////////////////////////