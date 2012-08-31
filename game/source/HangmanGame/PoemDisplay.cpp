#include "PoemDisplay.h"
#include "Platform.h"
#include "Game.h"
#include "GameLogic.h"
#include "Mesh.h"
#include "RenderPlane.h"
#include "LetterChooser.h"
#include <fstream>
////////////////////////////////////////////////////////////////////////////////
PoemDisplay::PoemDisplay(TextureFont* texFont)
:TextureText(texFont)
{
	mDisplayText = "Mistakes: 0, Max: 7";
	//SetString("FPS:  ");
	
	
}
////////////////////////////////////////////////////////////////////////////////
PoemDisplay::PoemDisplay(PersistentData& storageData)
:TextureText(storageData)
{
	std::string filename = storageData.GetProperty("file");
	height = storageData.GetPropertyF("height");
	width = storageData.GetPropertyF("width");
	mDuration = storageData.GetPropertyI("DisplayDuration");
	mAnimationDuration = storageData.GetPropertyI("FadeInDuration");
	ReadWordPoem(filename);
	mLastIdxShown = -1;
	mBGObjectTag = storageData.GetProperty("BackgroundTag");
	mBGObject = NULL;
	mFinished = false;
	//mDisplayText = "Mistakes: 0, Max: 7";
	//SetString(mDisplayText.c_str());
	
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::Serialize(PersistentData& storageData)
{
	TextureText::Serialize(storageData);
	storageData.SetName("PoemDisplay");
	
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::DeSerialize(PersistentData& storageData)
{
	TextureText::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::SetDisplayText(const char* s)
{
	mDisplayText = s;
	SetString(mDisplayText.c_str());
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::SetDisplayText(std::string s)
{
	mDisplayText = s;
	SetString(mDisplayText.c_str());
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::ReadWordPoem(std::string filename)
{
	//filename += "_marked_up.txt";
	//printf("Loading %s\n",filename.c_str());
	std::string fullpath;
	std::string file = filename;
	GetFullFilePathFromResource(file,fullpath);
	
	std::string wholeFile;
	std::string line;
	std::ifstream textstream(fullpath.c_str());
	while (std::getline(textstream, line)) 
	{
		wholeFile += line;
		wholeFile += "\n";
		mLines.push_back(line);
	} 
	textstream.close();
	SplitString(wholeFile, '|', mWords);
	
	
	//SetString("");
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::DisplayUpToWord(int idx)
{
	if(mWords.empty())
		return;
	
	if(idx >= mWords.size())
		idx = (int) mWords.size() - 1;
	
	
	if(mLastIdxShown != idx)
		mLastIdxShown = idx;
	
	std::string s;
	for(unsigned int i = 0; i <= idx; ++i)
	{
		s += mWords[i];
	}
	
	SetString(s.c_str());
	SetLastWordAlpha(0, mStartingGeometryIndexOfLastWord);
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::SetLastWordAlpha(const float a, unsigned int startIDx)
{
	
	for(unsigned int i = 0; i < mMeshes.size(); ++i)
	{
		GameID meshID = mMeshes[i];
		Mesh& m = RendererObjectFactory::GetMesh(meshID);
		
		m.Lock();
		for(unsigned int i = startIDx; i < m.mColorCount; ++i)
		{
			IvColor c = m.mColors[i];
			c.a = a;
			m.mColors[i] = c;
		}
		
		m.Unlock();
	}
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::Update()
{
	if(!mActive)
		return;
	
	unsigned int elapsed = GetTicks() - mStartTime;
	//float t = elapsed/(float)mDuration;
	float animT = elapsed/(float)mAnimationDuration;

	SetLastWordAlpha(animT, mStartingGeometryIndexOfLastWord);
	
	/*if(elapsed >= mDuration)
	{
		
		if(mCurrentWordIndex >= (int)mWords.size())
		{
			GameLogic::GetInstance().SetShouldShowLovePoem(false);
		}
		SendFinishMessage();
	}*/
	
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::MouseUp(float u, float v, int button)
{
	mStartTime = GetTicks();
	if(mCurrentWordIndex >= (int)mWords.size())
	{
		GameLogic::GetInstance().SetShouldShowLovePoem(false);
	}
	SendFinishMessage();
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::StartAsSequenceItem(PersistentData* storageData)
{		
	mBGObject = (RenderObject*)Game::GetInstance().GetTaggedObject(mBGObjectTag);
	mCurrentWordIndex = GameLogic::GetInstance().GetCurrentWordIndex()-1;
	bool skip = false;
	if(mCurrentWordIndex >= (int)mWords.size())
	{
		skip = true;
	}
	GameLogic::GetInstance().SetShouldShowLovePoem(!skip);
	
	
	
	if(skip || GameLogic::GetInstance().IsPlayingLostSequence())
	{
		if(mBGObject != NULL)
		{
			mBGObject->SetActive(false);
			mBGObject->SetVisible(false);
			//((RenderPlane*)mBGObject)->UnloadTexture();
		}
		GameLogic::GetInstance().ClearMetaballGrid();
		GameLogic::GetInstance().GetLetterChooser().Clear();
		SendFinishMessage();
		return;
	}
	
	GameLogic::GetInstance().ClearScreen();
	mStartTime = GetTicks();
	mActive = true;
	if(mBGObject != NULL)
		mBGObject->StartAsSequenceItem(storageData);
	
	//mCurrentWordIndex = (int)mWords.size();
	DisplayUpToWord(mCurrentWordIndex);
	
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::StopAsSequenceItem()
{
	mActive = false;
	if(mBGObject != NULL)
	{
		mBGObject->SetActive(false);
		mBGObject->SetVisible(false);
		mBGObject->StopAsSequenceItem();
		//mBGObject->SequenceOver(false);
		//((RenderPlane*)mBGObject)->UnloadTexture();
	}

}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::SequenceOver(bool deleteObject)
{
	//GameObject* go = Game::GetInstance().GetTaggedObject(mBGObjectTag);
	//if(go != NULL)
	//	go->SequenceOver();
}
////////////////////////////////////////////////////////////////////////////////
void PoemDisplay::SendFinishMessage()
{
	if(!mFinished)
	{
		mFinished = true;
		GameMessage msg;
		msg.mEventName = "FINISHED";
		msg.mSender = mID;
		SendOutMessage(msg);
	}
}
////////////////////////////////////////////////////////////////////////////////
