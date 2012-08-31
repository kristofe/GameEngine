#include "LetterManager.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "MetaballManager2D.h"

////////////////////////////////////////////////////////////////////////////////
LetterButton::LetterButton(unsigned int id, IvVector3& p, float r)
{
	mID = id;
	mPosition = p;
	mRadius = r;
	mMetaballGrid = NULL;
	//mMetaballGrid->AddObject(mID, mPosition, mRadius);
}
////////////////////////////////////////////////////////////////////////////////
LetterButton::~LetterButton()
{
	if(mMetaballGrid != NULL)
		mMetaballGrid->RemoveObject(mID);
}
////////////////////////////////////////////////////////////////////////////////
void LetterButton::RemoveFromMetaballGrid()
{
	if(mMetaballGrid != NULL)
		mMetaballGrid->RemoveObject(mID);
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterButton::AddToMetaballGrid(MetaballGrid* m)
{
	mMetaballGrid = m;
	if(mMetaballGrid != NULL)
		mMetaballGrid->AddObject(mID,mID, mPosition, mRadius);
	
}
////////////////////////////////////////////////////////////////////////////////
LetterManager::LetterManager(TextureFont* texFont):LetterDisplay(texFont)
{
	mMetaballGrid = NULL;
	mMetaballMgr2D = NULL;
	Init();
}
////////////////////////////////////////////////////////////////////////////////
LetterManager::LetterManager(PersistentData& storageData):LetterDisplay(storageData)
{
	PersistentData* textNode = storageData.GetNodeFromPath("AvailableLetters");
	mLettersCSV = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,0,1,2,3,4,5,6,7,8,9,_";
	if(textNode != NULL)
		mLettersCSV  = textNode->GetText();
	
	GenerateLetters(mLettersCSV );

}
////////////////////////////////////////////////////////////////////////////////
LetterManager::~LetterManager()
{
	for(int i = 0; i < (int)mSourceLetters.size(); ++i)
	{
		LetterButton* lb = mSourceLetters[i];
		SAFE_DELETE(lb);
	}
	mSourceLetters.clear();
	mActiveLetters.clear();
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::Init()
{
	mSelectedLetter = NULL;
	std::string meshName;
	meshName = "LetterManager";
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
void LetterManager::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("LetterManager");
	
	
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
void LetterManager::DeSerialize(PersistentData& storageData)
{
	SceneObject::DeSerialize(storageData);
	
	GameID fontID = static_cast<GameID> (storageData.GetPropertyU("Font"));
	TextureFont& font = Game::GetInstance().GetRenderer().GetTextureFont(fontID);
	mFont = &font;
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::Update()
{
	if(mActive == false)
		return;
	
	if(mDirtyMesh)
	{
		UpdateMeshFromLocalData();
	}

	if(mMetaballGrid != NULL)
	{
		LetterButton* lb;
		
		for(int i = 0; i < (int)mActiveLetters.size(); ++i)
		{
			lb = mActiveLetters[i];
			const IvVector3& p = lb->GetPosition();
			mMetaballGrid->EffectField(p.x, p.y, LETTER_METABALL_RADIUS);//lb->GetRadius());
		}
	}
	
	if(mMetaballMgr2D != NULL)
	{
		LetterButton* lb;
		
		for(int i = 0; i < (int)mActiveLetters.size(); ++i)
		{
			lb = mActiveLetters[i];
			const IvVector3& p = lb->GetPosition();
			mMetaballMgr2D->SetMetaballPosition(p, LETTER_METABALL_RADIUS_FOR_SPRITE);
		}
	}
	
}

////////////////////////////////////////////////////////////////////////////////
void LetterManager::GenerateLetters(std::string lettersCSV)
{	
	
	mVertices.clear();
	mColors.clear();
	mUVs.clear();
	mIndices.clear();
	mSourceLetters.clear();
	mActiveLetters.clear();
	std::vector<std::string> letterVec;
	SplitString(lettersCSV, ',', letterVec);
	int px = 0;
	int py = 0;
	for(int i = 0; i < letterVec.size(); ++i)
	{
		char c = *(letterVec[i].c_str());		
		LetterButton* letter = GenerateLetter(c,px,py);
		if(letter == NULL) continue;
		//px += 5 + c->xadv;
		mSourceLetters.push_back(letter);
		mActiveLetters.push_back(letter);

	}
	ConstructMeshFromLocalData();
}


////////////////////////////////////////////////////////////////////////////////
LetterButton* LetterManager::GenerateLetter(char targetLetter, int x, int y)
{	
	
	ToUpper(&targetLetter);
	TextureChar* c = &(mFont->chars[(int)targetLetter]);
	
	if (!c->available) return NULL;
	
	IvVector3 pos;
	pos.Set(x,y,0);
	LetterButton* letter = new LetterButton(targetLetter + 999000,pos,LETTER_METABALL_RADIUS);
	letter->mTextureChar = c;
	letter->mChar = targetLetter;
	//letter->SetRadius(LETTER_METABALL_RADIUS);
	float u1 = c->u1;
	float v1 = c->v1;
	float u2 = c->u2;
	float v2 = c->v2;
	/*int x1 = x + c->xo;
	int y1 = y - c->yo;
	int x2 = x1 + c->w;
	int y2 = y1 - c->h;*/

	int w = (int)(0.5f*c->w);
	int h = (int)(0.5f*c->h);
	int x1 = w;
	int y1 = h;
	int x2 = x1 + c->w;
	int y2 = y1 - c->h;
	
	FillInGeometryData(letter,x1,y1,x2,y2,u1,v1,u2,v2);
	
	//letter->SetPosition(pos);
	
	return letter;
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::FillInGeometryData(LetterButton* letter, float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
	
	int startIDX = mVertices.size();
	IvVector3 min, max;
	
	min.Set(x1 - TOUCH_RADIUS,y2 - TOUCH_RADIUS,-5);
	max.Set(x2 + TOUCH_RADIUS,y1 + TOUCH_RADIUS, 5);
	
	
	IvVector3 pos;
	pos.Set(x1,y1,0);
	mVertices.push_back(pos);
	pos.Set(x2,y1,0);
	mVertices.push_back(pos);
	pos.Set(x2,y2,0);
	
	mVertices.push_back(pos);
	pos.Set(x1,y2,0);
	mVertices.push_back(pos);
	
	IvColor c(1.0f,1.0f,1.0f,1.0f);
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
	
	int inidiceStart = mIndices.size();
	mIndices.push_back(startIDX + 0);
	mIndices.push_back(startIDX + 2);
	mIndices.push_back(startIDX + 3);
	
	mIndices.push_back(startIDX + 0);
	mIndices.push_back(startIDX + 1);
	mIndices.push_back(startIDX + 2);
	
	for(int i = 0; i <  4; ++i)
	{
		letter->mVertices[i] = startIDX + i;//&(mVertices[startIDX + i]);
		letter->mColors[i] = startIDX + i;//&(mColors[startIDX + i]);
		letter->mUVs[i] = startIDX + i;//&(mUVs[startIDX + i]);
		
	}
	letter->mAABB.Set(min, max);
	for(int i = 0; i <  6; ++i)
	{
		letter->mIndices[i] = (mIndices[inidiceStart + i]);

	}
	letter->CalcCenter();
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::AdjustMeshForActiveLetters()
{
	mIndices.clear();
	for(int i = 0; i < mActiveLetters.size(); ++i)
	{
		for(int j = 0; j < 6; ++j)
			mIndices.push_back(mActiveLetters[i]->mIndices[j]);
		
	}
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	m.mTriangleCount = mIndices.size()/3;
	SAFE_DELETE_ARRAY(m.mTriangles);
	m.mTriangles = new unsigned short[mIndices.size()];
	for(int i = 0; i < (int)mIndices.size(); i++)
	{
		m.mTriangles[i] = mIndices[i];
	}
	
	mIndices.clear();
	m.Unlock();
	
	
}
////////////////////////////////////////////////////////////////////////////////

void LetterManager::MouseUp(float u, float v, int button){
	if(!mActive)
		return;

	if(mSelectedLetter != NULL)
	{
		//IvColor c(1,1,1,1);
		//SetLetterColor(mSelectedLetter,c);
		//mDirtyMesh = true;
	}
	mSelectedLetter = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::MouseDown(float u, float v, int button)
{
	if(!mActive)
		return;

	mSelectedLetter = NULL;
	//if(button != 0) return;

	//Convert u and v into correct coords
	int x = u * mGame.GetRenderer().GetWindowWidth();
	int y = v * mGame.GetRenderer().GetWindowHeight();
	for(int i = 0; i < mActiveLetters.size(); i++)
	{
		LetterButton* lb = mActiveLetters[i];
		if(lb->HitTest((float)x,(float)y))
		{
			mSelectedLetter = lb;
			//IvColor c(1,0,0,1);
			//SetLetterColor(mSelectedLetter,c);
			//mDirtyMesh = true;
			//printf("selected letter %c\n", lb->mChar);
			break;
		}
		
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::MouseDragged(float u, float v,float lu, float lv, int button){
	//if(!mActive)
		return;

	if(mSelectedLetter != NULL){
		//Move the letter!
		float dx = (u - lu) * mGame.GetRenderer().GetWindowWidth();
		float dy = (v - lv) * mGame.GetRenderer().GetWindowHeight();
		//int x = u * mGame.GetRenderer().GetWindowWidth();
		//int y = v * mGame.GetRenderer().GetWindowHeight();
		IvColor c(1,1,1,1);
		MoveLetter(mSelectedLetter,dx,dy);
		//printf("Moving letter %c by %3.2f,%3.2f\n",mSelectedLetter->mChar, dx,dy);
		mDirtyMesh = true;
		
	}
}

////////////////////////////////////////////////////////////////////////////////
void LetterManager::SetLetterColor(LetterButton* letter, IvColor& c)
{
	for(int i = 0; i < 4; i++)
		mColors[letter->mColors[i]] = c;
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::MoveLetter(LetterButton* letter, int dx, int dy)
{
	IvVector3 d(dx,dy,0);
	for(int i = 0; i < 4; i++)
	{
		mVertices[letter->mVertices[i]] += d;
	}
	IvVector3 min, max;
	min = letter->mAABB.GetMinima();
	max = letter->mAABB.GetMaxima();
	min += d;
	max += d;
	letter->mAABB.Set(min,max);
	letter->CalcCenter();
	IvVector3 pos = letter->GetPosition();
	pos += d;
	letter->SetPosition(pos);
}
////////////////////////////////////////////////////////////////////////////////
void LetterManager::SetLetterPosition(LetterButton* letter, int x, int y)
{
	IvVector3 min, max;
	TextureChar* c = letter->mTextureChar;

	/*int x1 = x + c->xo;
	int y1 = y + c->yo;
	int x2 = x1 + c->w;
	int y2 = y1 - c->h;*/
	int w = (int)(0.5f*c->w);
	int h = (int)(0.5f*c->h);
	int x1 = x-w;
	int y1 = y+h;
	int x2 = x1 + c->w;
	int y2 = y1 - c->h;
	
	min.Set(x1 - TOUCH_RADIUS,y2 - TOUCH_RADIUS,-5);
	max.Set(x2 + TOUCH_RADIUS,y1 + TOUCH_RADIUS, 5);
	
	
	IvVector3 pos;
	pos.Set(x1,y1,0);
	mVertices[letter->mVertices[0]] = pos;
	pos.Set(x2,y1,0);
	mVertices[letter->mVertices[1]] = pos;
	pos.Set(x2,y2,0);
	mVertices[letter->mVertices[2]] = pos;
	pos.Set(x1,y2,0);
	mVertices[letter->mVertices[3]] = pos;
	
	letter->mAABB.Set(min,max);
	letter->CalcCenter();
	pos.Set(x,y,0);
	letter->SetPosition(pos);
}
////////////////////////////////////////////////////////////////////////////////