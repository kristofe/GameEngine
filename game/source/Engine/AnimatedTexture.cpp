#include "Game.h"
#include "Platform.h"
#include "AnimatedTexture.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "Mesh.h"
#include "Material.h"

////////////////////////////////////////////////////////////////////////////////
AnimationData2D::AnimationData2D()
{
	Init();	
}
////////////////////////////////////////////////////////////////////////////////
AnimationData2D::AnimationData2D(PersistentData& storageData)
{
	Init();
	animationName = storageData.GetTextOnlyChildAsText("AnimationName");
	textureCount =  storageData.GetTextOnlyChildAsInt("TextureCount");
	isPVR =  storageData.GetTextOnlyChildAsBool("IsPVR");
	colsPerTexture = storageData.GetTextOnlyChildAsInt("ColsPerTexture");
	rowsPerTexture = storageData.GetTextOnlyChildAsInt("RowsPerTexture");
	cellsPerTexture = storageData.GetTextOnlyChildAsInt("CellsPerTexture");
	if(cellsPerTexture == 0)
	{
		cellsPerTexture = colsPerTexture*rowsPerTexture;
	}
	startRow = storageData.GetTextOnlyChildAsInt("StartRow");
	startCol = storageData.GetTextOnlyChildAsInt("StartCol");
	totalCellsAcrossAllTextures = storageData.GetTextOnlyChildAsInt("TotalCellsAcrossAllTextures");
	fps = storageData.GetTextOnlyChildAsFloat("FPS");
	duration = storageData.GetTextOnlyChildAsFloat("Duration");
	dopeSheet = storageData.GetTextOnlyChildAsText("DopeSheet");
	ReadDopeSheet();
}

////////////////////////////////////////////////////////////////////////////////
void AnimationData2D::Init()
{
	textureCount = colsPerTexture =	rowsPerTexture = startRow =
	startCol = totalCellsAcrossAllTextures = fps = duration = 0;
	isPVR = false;
}

////////////////////////////////////////////////////////////////////////////////
std::string AnimationData2D::ToString()
{
	std::string s;
	char buff[1024];
	sprintf(buff,
		"AnimName: %s\nTextureCount: %d\nColsPerTexture: %d\nRowsPerTexture: %d\nCellsPerTexture: %d\nStartRow: %d\nStartCol: %d\nTotalCells: %d\nFPS: %3.0f\nTimeout: %3.2f"
		,animationName.c_str(),textureCount, colsPerTexture,rowsPerTexture,cellsPerTexture,startRow,startCol,totalCellsAcrossAllTextures,fps,duration
		);
	s = buff;
	return s;
}
////////////////////////////////////////////////////////////////////////////////
AnimationData2D&	AnimationData2D::operator=(const AnimationData2D &m) {
	if(this == &m) return *this;

	animationName = m.animationName;
	textureCount = m.textureCount;
	isPVR = m.isPVR;
	colsPerTexture = m.colsPerTexture;
	rowsPerTexture = m.rowsPerTexture;
	cellsPerTexture = m.cellsPerTexture;
	startRow = m.startRow;
	startCol = m.startCol;
	totalCellsAcrossAllTextures = m.totalCellsAcrossAllTextures;
	fps = m.fps;
	dopeSheet = m.dopeSheet;
	duration = m.duration;

	frames.clear();
	for(int i = 0; i < m.frames.size(); i++)
	{
		frames.push_back(m.frames[i]);

	}
	 return *this;
}
////////////////////////////////////////////////////////////////////////////////
void AnimationData2D::ReadDopeSheet()
{
	frames.clear();
    if (dopeSheet.length() > 0)
    {
		std::string c = ",";
		std::vector<std::string> strings = SplitString(dopeSheet,',');
        //frames = new int[strings.size()];
        for (int i = 0; i < strings.size(); ++i)
        {
            int idx = atoi(strings[i].c_str());
            if (idx >= totalCellsAcrossAllTextures)
            {
                idx = 0;
            }
			frames.push_back(idx);
        }
    }
    else
    {
        //frames = new int[totalCells];
        for (int j = 0; j < totalCellsAcrossAllTextures; ++j)
        {
			frames.push_back(j);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
PersistentData& AnimationData2D::Serialize(PersistentData& parent)
{
	PersistentData* c = new PersistentData("AnimationData2D");
	c->AddTextOnlyChild("AnimationName",animationName.c_str());
	c->AddTextOnlyChild("TextureCount",textureCount);
	c->AddTextOnlyChild("IsPVR",isPVR);
	c->AddTextOnlyChild("ColsPerTexture",colsPerTexture);
	c->AddTextOnlyChild("RowsPerTexture",rowsPerTexture);
	c->AddTextOnlyChild("CellsPerTexture",cellsPerTexture);
	c->AddTextOnlyChild("StartRow",startRow);
	c->AddTextOnlyChild("StartCol",startCol);
	c->AddTextOnlyChild("TotalCellsAcrossAllTextures",totalCellsAcrossAllTextures);
	c->AddTextOnlyChild("FPS",fps);
	c->AddTextOnlyChild("Duration",duration);
	c->AddTextOnlyChild("DopeSheet",dopeSheet.c_str());
	parent.AddChild(c);
	return *c;
}

////////////////////////////////////////////////////////////////////////////////
void AnimationData2D::DeSerialize(PersistentData& parent)
{
	std::string n = "AnimationData2D";
	PersistentData* c = parent.GetUniqueChild(n);
	if(c != NULL)
	{
		animationName = c->GetTextOnlyChildAsText("AnimationName");
		textureCount =  c->GetTextOnlyChildAsInt("TextureCount");
		isPVR =  c->GetTextOnlyChildAsBool("IsPVR");
		colsPerTexture = c->GetTextOnlyChildAsInt("ColsPerTexture");
		rowsPerTexture = c->GetTextOnlyChildAsInt("RowsPerTexture");
		cellsPerTexture = c->GetTextOnlyChildAsInt("CellsPerTexture");
		startRow = c->GetTextOnlyChildAsInt("StartRow");
		startCol = c->GetTextOnlyChildAsInt("StartCol");
		totalCellsAcrossAllTextures = c->GetTextOnlyChildAsInt("TotalCellsAcrossAllTextures");
		fps = c->GetTextOnlyChildAsFloat("FPS");
		duration = c->GetTextOnlyChildAsFloat("Duration");
		dopeSheet = c->GetTextOnlyChildAsText("DopeSheet");
	}
	
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
AnimatedTexture::AnimatedTexture(AnimationData2D ad)
{
	Init(ad);
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::AddEventListener(std::string eventName, GameID listener)
{
	std::string name = ToUpper(eventName);
	std::pair<std::string,GameID> p(name,listener);
	
	//printf("Adding event listener to Animated texture %s for event: %s, listener: %d.\n",mAnimationName.c_str(),eventName.c_str(),(int)listener);
	mMessageListeners.insert(std::pair<std::string,GameID>(ToUpper(eventName),listener));
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::Init(AnimationData2D& ad)
{
	mHalfWay = false;
	mDataLoaded = false;
	mCurrentFrameIdx = -1;
	mPaused = true;
	mCurrentTextureIdx = 0;
	mAnimationName = ad.animationName;
	mBasePath = "animations/";
	mBasePath += mAnimationName;
	mStartTime = 0;
	mUV = NULL;
	mAnimationData = ad;
	//mAnimationData.ReadDopeSheet();
	mDU = 1.0f/mAnimationData.colsPerTexture;
	mDV = 1.0f/mAnimationData.rowsPerTexture;
	
	
};
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::LoadData()
{
	for(int i = 0; i < mAnimationData.textureCount; i++)
	{
		char texName[128];
		if(mAnimationData.isPVR)
			sprintf(texName,"%s/%s_%02d.pvr",mBasePath.c_str(),mAnimationName.c_str(),i);
		else
			sprintf(texName,"%s/%s_%02d.png",mBasePath.c_str(),mAnimationName.c_str(),i);

		GameID texID = RendererObjectFactory::CreateTexture(texName,texName);
		mTextures.push_back(texID);
		mGame.GetRenderer().Retain(mID,texID);
		Texture& t = RendererObjectFactory::GetTexture(texID);
		mGame.GetRenderer().UploadTextureDataToGPU(t);
	}
	
	GameID meshID = RendererObjectFactory::CreateGridMesh(mAnimationName.c_str());
	GameID matID = RendererObjectFactory::CreateMaterial(mAnimationName.c_str());
	
	Mesh& mesh = RendererObjectFactory::GetMesh(meshID);
	//Create Local Copy of mesh data for quick access and for bulk update calls to mesh
	mUV = new IvVector2[mesh.mUVCount];
	memcpy(mUV,mesh.mUV,sizeof(IvVector2) * mesh.mUVCount);


	mMeshes.push_back(meshID);
	mMaterials.push_back(matID);
	Material& mat = RendererObjectFactory::GetMaterial(matID);
	mCurrentTextureID = mTextures[0];
	mat.AddTexture(mCurrentTextureID);
	

	mGame.GetRenderer().AddRenderObject(mID);
	mGame.GetRenderer().Retain(mID,meshID);
	mGame.GetRenderer().Retain(mID,matID);
	
	mDataLoaded = true;
	
	
	//Hack!  to prevent showing the entire texture at start
	float u1 = 0*mDU;
	float u2 = u1 + mDU;
    
    float v1 = 0*mDV;
	float v2 = v1 + mDV;
	
	mUV[0].Set(u1	,v2);
	mUV[1].Set(u2	,v2);
	mUV[2].Set(u1	,v1);
	mUV[3].Set(u2	,v1);
	
	Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
	m.Lock();
	m.CopyInUVs(mUV);
	m.Unlock();
	
	

}

////////////////////////////////////////////////////////////////////////////////
AnimatedTexture::AnimatedTexture(PersistentData& storageData):RenderObject(storageData)
{
	mHalfWay = false;
	mLoop = storageData.GetPropertyB("Loop");
	//mIsTrasparent = true;
	mPlayImmediately = storageData.GetPropertyB("PlayImmediately");
	PersistentData* node = storageData.GetNodeFromPath("AnimationData");
	AnimationData2D ad(*node);
	Init(ad);
	if(mPlayImmediately)
	{
		mActive = true;
		mVisible = true;
		GotoAndPlay(0);
	}
	
}
////////////////////////////////////////////////////////////////////////////////
AnimatedTexture::~AnimatedTexture()
{
	UnloadData();
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::UnloadData()
{
	if(!mDataLoaded) return;
	mHalfWay = false;
	
	mGame.GetRenderer().RemoveRenderObject(mID);
	for(int i = 0; i < (int)mMeshes.size(); ++i)
	{
		mGame.GetRenderer().Release(mID,mMeshes[i]);
		Material& m = RendererObjectFactory::GetMaterial(mMaterials[i]);
		
		mGame.GetRenderer().Release(mID,mMaterials[i]);
		m.ClearTextures();
	}
	for(unsigned int j = 0; j < mTextures.size(); j++)
	{
		mGame.GetRenderer().Release(mID,mTextures[j]);
	}
	SAFE_DELETE_ARRAY(mUV);
	mMeshes.clear();
	mMaterials.clear();
	mTextures.clear();
	mDataLoaded = false;
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::Serialize(PersistentData& storageData)
{
	RenderObject::Serialize(storageData);
	storageData.SetName("AnimatedTexture");
	storageData.SetProperty("Paused",mPaused);
	storageData.SetProperty("StartTime",mStartTime);
	storageData.SetProperty("CurrentFrameIdx",mCurrentFrameIdx);
	storageData.SetProperty("CurrentTextureIdx",mCurrentTextureIdx);
	storageData.SetProperty("AnimationName",mAnimationName.c_str());
	storageData.SetProperty("BasePath",mBasePath.c_str());
	storageData.SetProperty("DU",mDU);
	storageData.SetProperty("DV",mDV);
	storageData.SetProperty("CurrentTextureID",mCurrentTextureID);

	
	char buff[128];
	unsigned int objID;
	
	PersistentData* textures = new PersistentData("Textures");
	storageData.AddChild(textures);
	
	for(int i = 0; i < (int)mTextures.size(); ++i)
	{
		PersistentData* c = new PersistentData("Texture");
		objID = static_cast<unsigned int>(mTextures[i]);
		sprintf(buff, "%d",objID);
		std::string s = buff;
		c->SetTextProperty(s);
		textures->AddChild(c);
	}
	
	mAnimationData.Serialize(storageData);
	
	
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::DeSerialize(PersistentData& storageData)
{
	RenderObject::DeSerialize(storageData);
	
	mAnimationData.DeSerialize(storageData);
	mAnimationData.ReadDopeSheet();
	mPaused				= storageData.GetPropertyB("Paused");
	mStartTime			= storageData.GetPropertyU("StartTime");
	mCurrentFrameIdx	= storageData.GetPropertyI("CurrentFrameIdx");
	mCurrentTextureIdx  = storageData.GetPropertyF("CurrentTextureIdx");
	mAnimationName		= storageData.GetProperty("mAnimationName");
	mBasePath			= storageData.GetProperty("mBasePath");
	mDU					= storageData.GetPropertyF("DU");
	mDV					= storageData.GetPropertyF("DV");
	mCurrentTextureID	= static_cast<GameID> (storageData.GetPropertyU("CurrentTextureID"));
		
	std::list<PersistentData*> children;
		
	storageData.GetNodesFromPath("Textures",children);
	std::list<PersistentData*>::iterator it = children.begin();
	while(it != children.end())
	{
		PersistentData*c = (*it);
		GameID texID = static_cast<GameID> (c->GetTextAsUInt());
		mTextures.push_back(texID);
		//mGame.GetRenderer().Retain(mID,texID);
		Texture& t = RendererObjectFactory::GetTexture(texID);
		mGame.GetRenderer().UploadTextureDataToGPU(t);
		++it;
	}
	
	GameID meshID = mMeshes[0];
	Mesh& mesh = RendererObjectFactory::GetMesh(meshID);
	//Create Local Copy of mesh data for quick access and for bulk update calls to mesh
	mUV = new IvVector2[mesh.mUVCount];
	memcpy(mUV,mesh.mUV,sizeof(IvVector2) * mesh.mUVCount);
}

////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::Update()
{
	if(mPaused || !mActive) return;
	Animate();

	if(mDataLoaded)
	{
		Mesh& m = mGame.GetRenderer().GetMesh(mMeshes[0]);
		m.Lock();
		m.CopyInUVs(mUV);
		m.Unlock();
	}
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::Animate()
{
	//if(mStartTime == -1) mStartTime = GetTicks();
	unsigned int time = GetTicks();
	
	//Hack!!!
	//TODO: Make sure that the texture doesn't show before the uv's are set!
	if(mVisible != true)
		mVisible = true;
	
	float elapsed = (time - mStartTime)/1000.0f;
		//if(animTimeout > 0 && elapsed >= animTimeout){
		//	if(eventListener != null){
	    //		eventListener.SendMessage("AnimationEvent","MSG_ANIMATION_TIMED_OUT");
	    //	}
		//}
		
    // Calculate index
	int index = (int)((elapsed) * mAnimationData.fps);
	
    // Repeat when exhausting all cells
    index = index % mAnimationData.frames.size();
    
    if(index == mCurrentFrameIdx)// && !directionDirty){
    {
        return;	
    }
	if(index == (mAnimationData.frames.size()/2)-3)//hacked for specific screwey animation - it is the only one that used the event
	{
		if(mHalfWay == false)
		{
			mHalfWay = true;
			GameMessage msg;
			msg.mEventName = "HALF_WAY_POINT";
			msg.mSender = mID;
			SendOutMessage(msg);
		}
	}
    else if (index < mCurrentFrameIdx)
    {
		if(mLoop == false)
		{
			printf("AnimatedTexture %s finished.\n",mAnimationName.c_str());
			mPaused = true;
			GameMessage msg;
			msg.mEventName = "FINISHED";
			msg.mSender = mID;
			SendOutMessage(msg);
			
			
			return; //HOLD ANIMATION AT LAST FRAME
		}
		else
		{
			GameMessage msg;
			msg.mEventName = "LOOPED";
			msg.mSender = mID;
			SendOutMessage(msg);
		}
    }
    mCurrentFrameIdx = index;
	//printf("Playing frame...%d\n",index);
    
    int overallCell = mAnimationData.frames[index];
	int cell = overallCell % mAnimationData.cellsPerTexture;
	
	int textureSlot = (int)(overallCell/mAnimationData.cellsPerTexture);
	GameID desiredTexture = mTextures[textureSlot];
	if(mCurrentTextureID != desiredTexture)
	{
		mCurrentTextureID = desiredTexture;
		Material& mat = RendererObjectFactory::GetMaterial(mMaterials[0]);
		mat.SetMainTexture(mCurrentTextureID);

	}
    // split into horizontal and vertical index
	//if(mAnimationData.colsPerTexture == 0)
    //{
    //	mAnimationData.colsPerTexture = 1;
    //	printf("Caught a divide by zero early");	
    //}
	int uIndex = ((cell % mAnimationData.colsPerTexture)+mAnimationData.startCol)%mAnimationData.colsPerTexture;
    int vIndex = ((cell / mAnimationData.colsPerTexture)+mAnimationData.startRow)%mAnimationData.rowsPerTexture;
    
     
    float u1 = uIndex*mDU;
	float u2 = u1 + mDU;
    
    float v1 = vIndex*mDV;
	float v2 = v1 + mDV;

	mUV[0].Set(u1	,v2);
	mUV[1].Set(u2	,v2);
	mUV[2].Set(u1	,v1);
	mUV[3].Set(u2	,v1);

}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::GotoAndPlay(int frameNumber)
{
	if(!mDataLoaded)
	{
		LoadData();
	}
	int time = GetTicks();
	mStartTime = time - ((1000.0f*frameNumber)/mAnimationData.fps);
	mCurrentFrameIdx = frameNumber -1;
	mPaused = false;
	
}

////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::StartAsSequenceItem(PersistentData* storageData)
{
	mActive = true;
	GotoAndPlay(0);
}
////////////////////////////////////////////////////////////////////////////////
void AnimatedTexture::StopAsSequenceItem()
{
	mActive = false;
	mVisible = false;
	mPaused = true;
	UnloadData();
}
////////////////////////////////////////////////////////////////////////////////