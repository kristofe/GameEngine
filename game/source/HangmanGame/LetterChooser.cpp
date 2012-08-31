#include "LetterChooser.h"
#include "LetterManager.h"
#include "Game.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "GameLogic.h"
#include "Perlin.h"
////////////////////////////////////////////////////////////////////////////////
LetterChooser::LetterChooser(TextureFont* texFont,IvVector2& displayMin, IvVector2& displayMax, LayoutScheme layoutScheme, float jitterStrength):LetterManager(texFont)
{
	mDisplayMin = displayMin;
	mDisplayMax = displayMax;
	mLayoutScheme = layoutScheme;
	mJitterStrength = jitterStrength;
	mNextClickActivates = false;
}
////////////////////////////////////////////////////////////////////////////////
LetterChooser::LetterChooser(PersistentData& storageData):LetterManager(storageData)
{
	mGameLogicTag = storageData.GetProperty("GameLogicTag");
	//if(storageData.GetPropertyB("RefsAreImmediate"))
	//{
	//	mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	//}
	mDisplayMin = storageData.GetVector2Child("DisplayMin");
	mDisplayMax = storageData.GetVector2Child("DisplayMax");
	PersistentData* node = storageData.GetNodeFromPath("LayoutScheme");
	
	std::string method = node->GetProperty("method");
	method = ToUpper(method);
	if(method == "GRID")
	{
		mLayoutScheme = GRID;
	}
	else if(method == "SINE")
	{
		mLayoutScheme = SINE;
	}
	else
	{
		mLayoutScheme = RANDOM;
	}
	mJitterStrength = node->GetPropertyF("jitterStrength");
	mRows			= node->GetPropertyI("rows");
	mNextClickActivates = false;
	//CalculatePositions();
	//PlaceLetters();

}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::LevelLoaded()
{
	LetterManager::LevelLoaded();
	mGameLogic = static_cast<GameLogic*>(mGame.GetTaggedObject(mGameLogicTag));
	CalculatePositions();
	PlaceLetters();
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::CalculatePositions()
{

	switch(mLayoutScheme)
	{
		case GRID:
			GridLayout();
			break;
		case SINE:
			SineLayout();
			break;
		case RANDOM:
		default:
			RandomLayout();
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::PlaceLetters()
{
	/*for(unsigned int i = 0; i < mPositionList.size(); ++i)
	{
		IvVector3 pos = mPositionList[i];
		LetterButton* l = mActiveLetters[i];
		SetLetterPosition(l,pos.x,pos.y);
		
	}*/
	
	for(unsigned int i = 0; i < mActiveLetters.size(); ++i)
	{
		LetterButton* l = mActiveLetters[i];
		IvVector3 pos = mPositionDB[l->mChar];
		SetLetterPosition(l,pos.x,pos.y);
		
	}
	mDirtyMesh = true;
}

////////////////////////////////////////////////////////////////////////////////
void LetterChooser::GridLayout()
{
	std::vector<IvAABB> blockedAreas;
	mGameLogic->GetBlockedDropletAreas(blockedAreas);
	
	mPositionList.clear();
	//float w = mGame.GetRenderer().GetWindowWidth();
	//float h = mGame.GetRenderer().GetWindowHeight();
	float x,y;
	x = mDisplayMin.x;
	y = mDisplayMin.y;

	int lettersPerRow = mSourceLetters.size()/mRows;
	IvVector2 dim = mDisplayMax - mDisplayMin;
	int yIncrement = dim.y/mRows;
	int xIncrement = dim.x/lettersPerRow;

	IvVector3 origin;
	GetWorldPosition(origin);
	for(int i = 0; i < mSourceLetters.size(); ++i)
	{
		LetterButton* l = mSourceLetters[i];
		IvVector3 pos;
		pos.Set(x,y,origin.z);
		mPositionList.push_back(pos);
		mPositionDB[l->mChar] = pos;
		x += xIncrement;
		if(x >= mDisplayMax.x)
		{
			y += yIncrement;
			x = mDisplayMin.x;	
		}
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::RandomLayout()
{
	std::vector<IvAABB> blockedAreas;
	mGameLogic->GetBlockedDropletAreas(blockedAreas);
	
	mPositionList.clear();
	float x,y;
	x = mDisplayMin.x;
	y = 480.0f -  mDisplayMin.y;
	
	IvVector2 dim = mDisplayMax - mDisplayMin;
	int increment = 2.0f*TOUCH_RADIUS + mJitterStrength;
	
	IvVector3 origin;
	GetWorldPosition(origin);
	srand(1972);
	for(int i = 0; i < mSourceLetters.size(); ++i)
	{
		LetterButton* l = mSourceLetters[i];
		IvVector3 pos;
		
		float xNoise = rand() % (int)mJitterStrength;
		float yNoise = rand() % (int)mJitterStrength;
		
		
		pos.Set(x+xNoise,480.0f - (y+yNoise),origin.z);
		for(unsigned int j = 0; j < blockedAreas.size(); j++)
		{
			float yradius = l->mRadius*480.0f*0.5f;
			float xradius = l->mRadius*320.0f*0.5f;
			IvAABB blocker = blockedAreas[j];
			IvVector3 min, max;
			min.x = pos.x - xradius;
			min.y = pos.y - yradius;
			min.z = -5.0f;
			
			max.x = pos.x + xradius;
			max.y = pos.y + yradius;
			max.z = +5.0f;
			
			IvAABB test(min,max);
			
			if(test.Intersect(blocker))
			{
				float newX = blocker.GetMaxima().x + xradius;
				if(newX > x)
				{
					x = newX;
					if(x >= mDisplayMax.x)
					{
						y += increment;
						pos.y -= increment;
						x = mDisplayMin.x;	
					}
					pos.x = x;
				}
			}
			
		}
		mPositionList.push_back(pos);
		mPositionDB[l->mChar] = pos;
		x += increment;
		if(x >= mDisplayMax.x)
		{
			y += increment;
			x = mDisplayMin.x;	
		}
	}
	
}
////////////////////////////////////////////////////////////////////////////////
LetterChooser::~LetterChooser()
{

}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::GetChars(std::vector<char>& chars)
{
	for(unsigned int i = 0; i < mSourceLetters.size(); ++i)
	{
		chars.push_back(mSourceLetters[i]->mChar);		
	}
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::GetLetterPositions(std::vector<IvVector3>& positions)
{
	for(unsigned int i = 0; i < mPositionList.size(); ++i)
	{
		positions.push_back(mPositionList[i]);		
	}
}
////////////////////////////////////////////////////////////////////////////////
const char* LetterChooser::MethodToString()
{
	if(mLayoutScheme == GRID)
	{
		return "GRID";
	}
	else if(mLayoutScheme == SINE)
	{
		return "SINE";
	}
	else
	{
		return "RANDOM";
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::Serialize(PersistentData& storageData)
{
	LetterManager::Serialize(storageData);
	storageData.SetName("LetterChooser");
	storageData.AddVector2Child(mDisplayMin,"DisplayMin");
	storageData.AddVector2Child(mDisplayMax,"DisplayMax");
	PersistentData* n = new PersistentData("LayoutScheme","LayoutScheme");
	storageData.AddChild(n);
	n->SetProperty("method",MethodToString());
	n->SetProperty("jitterStrength",mJitterStrength);
	n->SetProperty("rows",mRows);

}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::DeSerialize(PersistentData& storageData)
{
	LetterManager::DeSerialize(storageData);

	

	/*
		mDisplayMin = storageData.GetVector2Child("DisplayMin");
	mDisplayMax = storageData.GetVector2Child("DisplayMax");
	PersistentData* node = storageData.GetNodeFromPath("LayoutScheme");

	std::string method = node->GetProperty("method");
	method = ToUpper(method);
	if(method == "GRID")
	{
		mLayoutScheme = GRID;
	}
	else if(method == "SINE")
	{
		mLayoutScheme = SINE;
	}
	else
	{
		mLayoutScheme = RANDOM;
	}
	mJitterPower = node->GetPropertyF("jitterStrength");
	*/

	
}
////////////////////////////////////////////////////////////////////////////////

void LetterChooser::MouseUp(float u, float v, int button){
	if(mNextClickActivates)
	{
		mNextClickActivates = false;
		mActive = true;
		return;
	}

	if(GameLogic::GetInstance().AllowChoosing())
	{
		if(!mActive)
			return;

		if(mSelectedLetter != NULL)
		{
			LetterButton* b = mSelectedLetter;
			mLastLetterChosen = b;
			std::vector<LetterButton*>::iterator it = mActiveLetters.begin();
			while(it != mActiveLetters.end())
			{
				if((*it) == mSelectedLetter)
				{
					mSelectedLetter->RemoveFromMetaballGrid();
					mActiveLetters.erase(it);
					break;
				}
				++it;
			}
					
			LetterManager::MouseUp(u,v,button);


			LetterChooserMessage msg;
			msg.mEventName = "LETTER_CHOSEN";
			msg.mSender = mID;
			msg.mLetterButton = b;
			SendOutMessage(msg);
			AdjustMeshForActiveLetters();
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::AdjustLayout()
{

	ConstructMeshFromLocalData();
	CalculatePositions();//The functions involved have to respond to only active letters... but need to respond to source letters at startup
	PlaceLetters();
	AdjustMeshForActiveLetters();
	for(int i = 0; i < (int)mSourceLetters.size(); ++i)
	{
		mSourceLetters[i]->RemoveFromMetaballGrid();
	}
	//mMetaballGrid->ClearGridValues();
	for(int i = 0; i < (int)mActiveLetters.size(); ++i)
	{
		mActiveLetters[i]->AddToMetaballGrid(mMetaballGrid);
	}
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::Reset()
{

	mActiveLetters.clear();
	for(int i = 0; i < (int)mSourceLetters.size(); ++i)
	{
		mActiveLetters.push_back(mSourceLetters[i]);
	}
	ConstructMeshFromLocalData();
	CalculatePositions();
	PlaceLetters();
	AdjustMeshForActiveLetters();
	for(int i = 0; i < (int)mSourceLetters.size(); ++i)
	{
		mSourceLetters[i]->RemoveFromMetaballGrid();
	}
	//mMetaballGrid->ClearGridValues();
	for(int i = 0; i < (int)mActiveLetters.size(); ++i)
	{
		mActiveLetters[i]->AddToMetaballGrid(mMetaballGrid);
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void LetterChooser::Clear()
{
	for(int i = 0; i < (int)mSourceLetters.size(); ++i)
	{
		mSourceLetters[i]->RemoveFromMetaballGrid();
	}
	mActiveLetters.clear();
	ConstructMeshFromLocalData();
	CalculatePositions();
	//LayoutLetters();
	AdjustMeshForActiveLetters();
	
}
////////////////////////////////////////////////////////////////////////////////
IvVector3 LetterChooser::GetLetterPosition(char c)
{
	IvVector3 pos(0,0,0);
	std::map<char,IvVector3>::iterator it = mPositionDB.find(c);
	if(it != mPositionDB.end())
	{
		pos = it->second;
	}
	return pos;
}

////////////////////////////////////////////////////////////////////////////////