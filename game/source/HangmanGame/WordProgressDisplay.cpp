#include "WordProgressDisplay.h"
#include "HangingLetterSign.h"
#include "LetterChooser.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "RenderTarget.h"

////////////////////////////////////////////////////////////////////////////////
WordProgressDisplay::WordProgressDisplay(TextureFont* texFont):RenderObject(),mFont(texFont)
{
	mLastChosenLetter = '\0';
}
////////////////////////////////////////////////////////////////////////////////
WordProgressDisplay::WordProgressDisplay(PersistentData& storageData):RenderObject(storageData)
{
	std::string fontName = storageData.GetProperty("Font");
	GameID fontID = RendererObjectFactory::CreateTextureFont(fontName.c_str());
	TextureFont& font = RendererObjectFactory::GetTextureFont(fontID);
	mFont = &font;
	mSpaceBetweenTiles = storageData.GetPropertyI("SpaceBetweenTiles");
	mRotationDuration = storageData.GetPropertyI("RotationDuration");
	mWindDuration = storageData.GetPropertyI("WindDuration");
	mRenderTargetTag = storageData.GetProperty("AddChildrenToRenderTarget");
	mLastChosenLetter = '\0';
	//SetTargetWord("Kristofer_Schlachter");

}
////////////////////////////////////////////////////////////////////////////////
WordProgressDisplay::~WordProgressDisplay()
{
	ClearTiles();
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::LevelLoaded()
{
	std::string s = "LetterChooser";
	GameObject* go = mGame.GetTaggedObject(s);
	if(go)
	{
		std::string e = "LETTER_CHOSEN";
		go->AddEventListener(e,mID);
	}
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::SetTargetWord(std::string s)
{
	mTargetWord = s;
	
	ClearTiles();
	GenerateTiles();
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::LetterChosen(char c)
{
	std::multimap<char,HangingLetterSign*>::iterator it = mLetterSlots.lower_bound(c);
	std::multimap<char,HangingLetterSign*>::iterator endIt = mLetterSlots.upper_bound(c);
	while(it != endIt)
	{
		it->second->SetChar(c);
		++it;
	}
	mLastChosenLetter = c;
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::BlowTilesAround()
{
	std::multimap<char,HangingLetterSign*>::iterator it = mLetterSlots.begin();
	std::multimap<char,HangingLetterSign*>::iterator endIt = mLetterSlots.end();
	while(it != endIt)
	{
		it->second->BlowAround();
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::GetLetterPositions(char c, std::vector<IvVector3>& positionList)
{
	std::multimap<char,HangingLetterSign*>::iterator it = mLetterSlots.lower_bound(c);
	std::multimap<char,HangingLetterSign*>::iterator endIt = mLetterSlots.upper_bound(c);
	while(it != endIt)
	{
		IvVector3 p;
		it->second->GetLocalPosition(p);
		p.y -= 25.0f;
		//p = mWorldTransform.Transform(p);
		positionList.push_back(p);
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::GenerateTiles()
{
	//RenderTarget* rt = NULL;
	//if(!mRenderTargetTag.empty())
	//	rt = (RenderTarget*)mGame.GetTaggedObject(mRenderTargetTag);
	
	IvVector3 pos(mLocalPosition);
	for(int i = 0; i < (int)mTargetWord.size(); ++i)
	{
		char c = mTargetWord[i];

		HangingLetterSign* ld = new HangingLetterSign(mFont,mRotationDuration,mWindDuration);
		//if(rt != NULL)
		//{
			//ld->SetExclusiveToRenderTarget(mExclusiveToRenderTarget);
			//rt->AddRenderObject(ld);
		//}
		ld->SetPosition(pos);
		//ld->SetChar('_');
		
		mLetterSlots.insert(std::pair<char,HangingLetterSign*>(c,ld));
		pos.x += (float)mSpaceBetweenTiles;
		mDisplayTiles.push_back(ld);
		
	}
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::ClearTiles()
{
	RenderTarget* rt = NULL;
	if(!mRenderTargetTag.empty())
		rt = (RenderTarget*)mGame.GetTaggedObject(mRenderTargetTag);
	
	for(int i = 0; i < (int)mDisplayTiles.size(); ++i)
	{
		HangingLetterSign* ld = mDisplayTiles[i];
		ld->SetActive(false);
		if(rt != NULL)
		{
			//rt->RemoveRenderObject(ld);
		}
		//ld->SetVisible(false);
		mGame.DeleteGameObject(ld->GetID());
		
		mDisplayTiles[i] = NULL;
	}
	mDisplayTiles.clear();
	mLetterSlots.erase(mLetterSlots.begin(),mLetterSlots.end());
	mLastChosenLetter = '\0';
}
////////////////////////////////////////////////////////////////////////////////
/*void WordProgressDisplay::SetDebugString(std::string s)
{
	mDebugString = s;
	
	GenerateLetters(ToCSV(mDebugString));

	float x,y;
	x = 0;
	y = 20;

	for(int i = 0; i < mActiveLetters.size(); ++i)
	{
		LetterButton* l = mActiveLetters[i];
		MoveLetter(l,x,y);
		x += l->mTextureChar->xadv + 3;
	}
	mDirtyMesh = true;
}*/
////////////////////////////////////////////////////////////////////////////////
std::string  WordProgressDisplay::ToCSV(std::string& s)
{
	std::string ret = "";
	int letterCount = (int)s.size();
	for(int i = 0; i < letterCount; ++i)
	{
		ret += s[i];
		if( i < letterCount - 1)
		{
			ret += ",";
		}
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::ShowWholeWord(std::string word)
{
	//char c = '\0';
	for(unsigned int i = 0; i < word.size(); ++i)
	{
		LetterChosen(word[i]);
	}
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("WordProgressDisplay");
	/*storageData.AddVector2Child(mDisplayMin,"DisplayMin");
	storageData.AddVector2Child(mDisplayMax,"DisplayMax");
	PersistentData* n = new PersistentData("LayoutScheme","LayoutScheme");
	storageData.AddChild(n);
	n->SetProperty("method",MethodToString());
	n->SetProperty("jitterStrength",mJitterStrength);
	n->SetProperty("rows",mRows);
	*/
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void WordProgressDisplay::ProcessMessage(GameMessage& msg)
{
	if(msg.mEventName == "LETTER_CHOSEN")
	{
		//LetterChooserMessage& m = static_cast<LetterChooserMessage&>(msg);
		//printf("Letter Chosen: %c\n",m.mLetterButton->mChar);
	}
}
////////////////////////////////////////////////////////////////////////////////
std::string WordProgressDisplay::DebugOutput()
{
	char buf[256];
	sprintf(buf, "\tDisplayTilesCount:%d\tLetterSlotsCount:%d\tFontName:%s\tFontID:%d",(int)mDisplayTiles.size(),(int)mLetterSlots.size(),mFont->GetName().c_str(),mFont->GetID());
	std::string superInfo = RenderObject::DebugOutput();
	std::string s = superInfo + buf;
	return s;
}
////////////////////////////////////////////////////////////////////////////////
