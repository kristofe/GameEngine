#include "HangingLetterSign.h"
#include "TextureFont.h"
#include "Game.h"
#include "Platform.h"
#include "Material.h"
#include "Mesh.h"
#include "Globals.h"
#include "MetaballGrid.h"
#include "Scene.h"
#include "Perlin.h"
#include "GameLogic.h"
////////////////////////////////////////////////////////////////////////////////
HangingLetterSign::HangingLetterSign(TextureFont* font, unsigned int rotationAnimationDuration, unsigned int windAnimationDuration):LetterDisplay(font),mLetterDisplay(NULL)
{
	mMetaballGrid = NULL;
	mActive = true;
	Init();
	mStartTime = GetTicks() - windAnimationDuration;
	mLetterDisplay = NULL;
	mRotationDuration = rotationAnimationDuration;
	mWindDuration = windAnimationDuration;
	mPendingChar = '\0';
	mUseWindAnimation = true;
	//SetChar(c);
	//mStartPos = start;
	//mEndPos = end;
	//mDuration = duration;

}
////////////////////////////////////////////////////////////////////////////////
HangingLetterSign::~HangingLetterSign()
{

}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::Init()
{
	std::string meshName;
	char buff[64];
	GameID signMeshID = 0;
	GameID matID = 0;
	GameID texID = 0;
	
	
	//Now create the main part of the sign
	meshName = "HangingLetterSign_SignMain";
	sprintf(buff,"%03d",mID);
	meshName += buff;
	signMeshID = RendererObjectFactory::CreateUnitCubeMesh();
	signMeshID = RendererObjectFactory::CloneMesh(signMeshID,meshName);
	mMeshes.push_back(signMeshID);//has to be added to this list  - in order to be released;
	
	Mesh& m = RendererObjectFactory::GetMesh(signMeshID);
	for(unsigned int i = 0; i < m.mVertexCount; ++i)
	{
		m.mVertices[i].x *= 25.0f;
		m.mVertices[i].y *= 35.0f;
		m.mVertices[i].y -= 35.0f;
		m.mVertices[i].z *= 5.0f;
		//m.mVertices[i].z -= 5.0f;
	}

	//FLIP the texture coords on the back so i can fake the rotation of the reveal
	m.mUV[4].Set(1,1);
	m.mUV[5].Set(0,1);
	m.mUV[6].Set(0,0);
	m.mUV[7].Set(1,0);
	
	//Darken the sides and bottom by referencing part of the texture
	for(int ii = 8; ii <= 23; ii++)
		m.mColors[ii].Set(0.5,0.5f,0.5f,1);
	//top face - x/z plane
	m.mUV[8].Set(0,1);
	m.mUV[9].Set(0,1);
	m.mUV[10].Set(0,0);
	m.mUV[11].Set(0,0);


	
	//bottom face x/z plane - reorder indices	
	m.mUV[12].Set(0,1);
	m.mUV[13].Set(0,1);
	m.mUV[14].Set(0,0);
	m.mUV[15].Set(0,0);

	
	//left face y/z plane	
	m.mUV[16].Set(0,1);
	m.mUV[17].Set(0,1);
	m.mUV[18].Set(0,0);
	m.mUV[19].Set(0,0);

	
	//right face y/z	
	m.mUV[20].Set(0,1);
	m.mUV[21].Set(0,1);
	m.mUV[22].Set(0,0);
	m.mUV[23].Set(0,0);
	


	char matName[256];
	std::string signMainImageName = "sign_plaque.png";
	sprintf(matName, "%s%03d",signMainImageName.c_str(),mID);
	matID = RendererObjectFactory::CreateMaterial(matName);
	texID = RendererObjectFactory::CreateTexture(signMainImageName.c_str(),signMainImageName.c_str());
	Material& matMain = RendererObjectFactory::GetMaterial(matID);
	matMain.SetMainTexture(texID);
	mMaterials.push_back(matID);

	mGame.GetRenderer().Retain(mID,texID);
	mGame.GetRenderer().Retain(mID,signMeshID);
	mGame.GetRenderer().Retain(mID,matID);
	//if(texID != 0)
	//{
	//	mGame.GetRenderer().Retain(mID,texID);
	//}
	
	//create the post for the sign
	meshName = "HangingLetterSign_SignPost";
	sprintf(buff,"%03d",mID);
	meshName += buff;
	GameID signPostMeshID = RendererObjectFactory::CreateUnitCubeMesh();
	signPostMeshID = RendererObjectFactory::CloneMesh(signPostMeshID,meshName);
	mMeshes.push_back(signPostMeshID);//has to be added to this list  - in order to be released;
	
	Mesh& m2 = RendererObjectFactory::GetMesh(signPostMeshID);
	for(unsigned int i = 0; i < m2.mVertexCount; ++i)
	{
		m2.mVertices[i].x *= 2.0f;
		m2.mVertices[i].y *= 17.5f;
		m2.mVertices[i].y -= 8.75f;
		m2.mVertices[i].z *= 2.5f;
		//m2.mVertices[i].z -= 1.25f;
	}

	//matID = RendererObjectFactory::GetDefaultMaterial();
	
	std::string signPostImageName = "sign_post.png";
	sprintf(matName, "%s%03d",signPostImageName.c_str(),mID);
	matID = RendererObjectFactory::CreateMaterial(matName);
	texID = RendererObjectFactory::CreateTexture(signPostImageName.c_str(),signPostImageName.c_str());
	Material& matPost = RendererObjectFactory::GetMaterial(matID);
	matPost.SetMainTexture(texID);
	mMaterials.push_back(matID);
	
	mGame.GetRenderer().Retain(mID,texID);
	
	mGame.GetRenderer().Retain(mID,signPostMeshID);
	mGame.GetRenderer().Retain(mID,matID);
	
	
	mGame.GetRenderer().AddRenderObject(mID);//This is where it is specified that it is text

	
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::Serialize(PersistentData& storageData)
{
	LetterDisplay::Serialize(storageData);
	storageData.SetName("HangingLetterSign");
	
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::DeSerialize(PersistentData& storageData)
{
	LetterDisplay::DeSerialize(storageData);
	
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::LateUpdate()
{
	
	if(mActive)
		Animate();
	
}
///////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::IdleAnimate()
{
	unsigned int time = GetTicks();
	IvVector3 pos;
	this->GetLocalPosition(pos);
	
	float noise = 0.2f* Perlin::Noise(time*0.001f,pos.x*0.01f);
	//float hiFreqNoise = 0.1f * Perlin::Noise(time*0.01f,pos.x*0.01f);
	mLastUpdateTime = time;
	
	this->ResetLocalTransform();//Start with identity
	this->Translate(pos);
	
	float negNoise = -(0.5f*noise + 0.5f);
	//negNoise *=5.0f;
	this->Rotate(noise*0.3f,noise*0.5f,negNoise*0.5f);
}

///////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::BlowAround()
{
	mUseWindAnimation = true;
	mStartTime = GetTicks() + 1800;//Hacked in delay for sound sync
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::SetChar(char c)
{
	mUseWindAnimation = false;
	IvVector3 pos;
	GetWorldPosition(pos);
	pos.x = (int) pos.x;
	pos.y = (int) pos.y;
	this->SetWorldPosition(pos);
	//LetterDisplay::SetChar(c);
	if(mLetterDisplay == NULL)
	{
		mLetterDisplay = new LetterDisplay(mFont);
		IvVector3 disp;
		//GetWorldPosition(disp);
		disp.z += 2.55f;
		disp.y -= 27;
		mLetterDisplay->Translate(disp);
		
		//Hack!  for letter "i" it is too sharp and thin
		if(c == 'I')
		{
			IvVector3 s(1.25f,1.0f,1.0f);
			mLetterDisplay->Scale(s);
		}
		this->AddChild(mLetterDisplay);

	}
	mPendingChar = c;
	mStartTime = GetTicks();
	int randTime =  (rand()/(float)RAND_MAX)*1000;
	mStartTime += randTime;
	//mLetterDisplay->SetChar(c);
	

	

}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::Animate()
{
	if(mUseWindAnimation)
		AnimateWind();
	else
		AnimateReveal();
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::AnimateWind()
{
	unsigned int time = GetTicks();
	unsigned int elapsed = time - mStartTime;
	IvVector3 pos;
	this->GetLocalPosition(pos);
	float t = elapsed/(float)mWindDuration;
	
	if(t > 1.0f)
	{
			//this->ResetLocalTransform();//Start with identity
			//this->Translate(pos);
			IdleAnimate();
			return;
			/*GameMessage msg;
			msg.mEventName = "FINISHED";
			msg.mSender = mID;
			SendOutMessage(msg);
			return;*/
	}

	float noise = 0.2f* Perlin::Noise(time*0.001f,pos.x*0.01f);
	float hiFreqNoise = Perlin::Noise(time*0.007f,pos.x*0.01f);
	//ramp noise based on proximity to start and end animation
	
	//have to start with 1.0f times noise because that is what was driving the position and motion of the sign before
	//this animation... if you didn't start with the regular noise value then you would get a hitch in position.
	float s = 2.0f;
	if(t <= 0.25f)
		s = LERP(t*4.0f,1.0f,2.0f);
	else if( t >= 0.75f)
		s = LERP((t-0.75f)*4.0f,2.0f,1.0f);

	mLastUpdateTime = time;
	
	this->ResetLocalTransform();//Start with identity
	this->Translate(pos);
	
	float negNoise = -(0.5f*noise + 0.5f);
	this->Rotate(s*noise*0.3,s*hiFreqNoise*0.5f,s*negNoise*0.5);
	
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::AnimateReveal()
{
	unsigned int time = GetTicks();
	int elapsed = time - (int)mStartTime;
	float t = elapsed/(float)mRotationDuration;
	
	if(t < 0.0)
		return;
	t = powf(t,0.33f);
	if(t > 1.0f)
	{
			mUseWindAnimation = true;
			mStartTime = 0;
			//GameLogic::GetInstance().SetAllowChoosing(true);
			/*
			GameMessage msg;
			msg.mEventName = "FINISHED";
			msg.mSender = mID;
			SendOutMessage(msg);
			return;
			*/
	}

	if(t >= 0.5f)
	{
		if(mLetterDisplay && mLetterDisplay->GetChar() != mPendingChar)
		{
			//printf("Setting Char\n");
			mLetterDisplay->SetChar(mPendingChar,false,IvColor::black);
		}
	}

		

	mLastUpdateTime = time;
	float rot = t * PI - PI;//basically 1.0 radians a second
	IvVector3 pos;
	this->GetLocalPosition(pos);
	this->ResetLocalTransform();//Start with identity
	this->Translate(pos);
	
	float noise = 0.2f* Perlin::Noise(time*0.001f,pos.x*0.01f);
	float negNoise = -(0.5f*noise + 0.5f);
	this->Rotate(noise*0.3f,rot + noise*0.5f,negNoise*0.5f);
	
	//this->Rotate(0.0f,rot,0.0f);
	
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::StartAsSequenceItem(PersistentData* storageData)
{
	mActive = true;
	mVisible = true;
	mStartTime = GetTicks();
	mUseWindAnimation = true;
	//SetPosition(mStartPos);
	//mGame.GetActiveScene()->AddGameObject(this);
	
}
////////////////////////////////////////////////////////////////////////////////
void HangingLetterSign::StopAsSequenceItem()
{
	mActive = false;
	//mGame.GetActiveScene()->RemoveGameObject(this);//Handled in Game::RemoveGameObject
}
////////////////////////////////////////////////////////////////////////////////