#include <string>
#include <iostream>
#include <fstream>
#include "Material.h"
#include "Game.h"
#include "RenderTarget.h"

////////////////////////////////////////////////////////////////////////////////
/* REMOVED THESE BECAUSE IT IS HARD TO TELL WHAT MESHES ARE ACTIVE IF WE TRACK ONLY 
   THROUGH MATERIAL
void Material::AddMesh(Mesh* m)
{
	std::map<RendererMeshID, Mesh*>::iterator it;
	it = mMeshes.find(m->GetGameID());
	if(it != mMeshes.end())
	{
		return;
	}
	mMeshes[m->GetGameID()] = m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RemoveMesh(Mesh* m)
{
	std::map<RendererMeshID, Mesh*>::iterator it;
	it = mMeshes.find(m->GetGameID());
	if(it != mMeshes.end())
	{
		mMeshes.erase(it);
	}

}
*/
////////////////////////////////////////////////////////////////////////////////
void Material::Configure(PersistentData& storageData)
{
	mUseRenderTarget = false;
	mDiffuse = storageData.GetColorChild("Diffuse");
	mSpecular = storageData.GetColorChild("Specular");
	mAmbient = storageData.GetColorChild("Ambient");
	mEmissive = storageData.GetColorChild("Emissive");
	mColorMask = storageData.GetColorChild("ColorMask");
	if(mColorMask != IvColor::white)
	{
		mUseColorMask = true;
	}
	
	mZWrite = !storageData.GetPropertyB("DisableZWrite");
	mZTest = !storageData.GetPropertyB("DisableZTest");

	PersistentData* texturesParent = storageData.GetNodeFromPath("Textures");
	if(texturesParent != NULL)
	{
		ClearTextures();
		std::list<PersistentData*> textures;
		texturesParent->GetNodesFromPath("Texture",textures);
		std::list<PersistentData*>::iterator txtIt = textures.begin();
		while(txtIt != textures.end())
		{
			PersistentData* txNode = *txtIt;
			//////////////////////////
			
			mRenderTargetTag = txNode->GetProperty("UseRenderTarget");
			if(mRenderTargetTag.empty() == false)
			{
				RenderTarget* rt = (RenderTarget*)Game::GetInstance().GetTaggedObject(mRenderTargetTag);
				if(rt != NULL)
				{
					mUseRenderTarget = true;
					AddTexture(rt->GetTextureID());
				}
				else
				{
					printf("ERROR: Material '%s' wants to use non-existent render target: %s\n",mName.c_str(),mRenderTargetTag.c_str());
				}
			}
			else
			{
				std::string imagename = txNode->GetProperty("ImageName");
				bool delayLoad = txNode->GetPropertyB("DelayLoad");
				GameID texID = RendererObjectFactory::CreateTexture(imagename.c_str(),imagename.c_str(),delayLoad);
				AddTexture(texID);				
			}
			//////////////////////////
			++txtIt;
		}
		
	}
	

	PersistentData* tmParent = storageData.GetNodeFromPath("TextureMatrices");
	if(tmParent != NULL)
	{
		std::list<PersistentData*> textureMatrices;
		tmParent->GetNodesFromPath("TextureMatrix",textureMatrices);
		
		IvVector3 pos, scale, rot;
		int idx = 0;
		std::list<PersistentData*>::iterator tmNodeIt = textureMatrices.begin();
		while(tmNodeIt != textureMatrices.end())
		{
			PersistentData* tmNode =*tmNodeIt;
			if(tmNode != NULL){
				//mUseTextureMatrix = true;
				if(tmNode->GetNodeFromPath("Position") != NULL)
				{
					pos = tmNode->GetVector3Child("Position");
					TranslateTextureMatrix(pos,idx);
				}
				if(storageData.GetNodeFromPath("Rotation") != NULL)
				{
					rot = tmNode->GetVector3Child("Rotation");
					RotateTextureMatrix(rot.x,rot.y,rot.z,idx);
				}
				if(tmNode->GetNodeFromPath("Scale") != NULL)
				{
					scale = tmNode->GetVector3Child("Scale");
					ScaleTextureMatrix(scale,idx);
				}
			}
			++idx;
			++tmNodeIt;
		}
		
	}
	
	PersistentData* tc = storageData.GetNodeFromPath("TextureCombiners");
	if(tc != NULL)
	{
		ConfigureCombiners(*tc);
	}
	
	std::string blendFunc;
	Renderer& rend = Game::GetInstance().GetRenderer();
	
	PersistentData* blend = storageData.GetNodeFromPath("Blend");
	mBlend = blend->GetPropertyB("Enabled");

	blendFunc = blend->GetProperty("FuncA");
	mBlendFuncA = rend.GetEnumForString(blendFunc);
	
	blendFunc = blend->GetProperty("FuncB");
	mBlendFuncB = rend.GetEnumForString(blendFunc);

	
	std::string alphaFunc;
	PersistentData* alphaTest = storageData.GetNodeFromPath("AlphaTest");
	if(alphaTest != NULL)
	{
		mUseAlphaTest = alphaTest->GetPropertyB("Enabled");
		mAlphaFuncTestValue = alphaTest->GetPropertyF("value");
		alphaFunc = alphaTest->GetProperty("Func");
		mAlphaFunc = rend.GetEnumForString(alphaFunc);
		
	}
	
	/*
	 //Causes Crash on 3.0 Release but not 3.0 debug
	 PersistentData* shaderInfo = storageData.GetNodeFromPath("Shaders");
	if(shaderInfo != NULL)
	{
		mVertexShaderPath = shaderInfo->GetProperty("vertex");
		mFragmentShaderPath = shaderInfo->GetProperty("fragment");
		SetupShaders();
	}*/
}

////////////////////////////////////////////////////////////////////////////////
void Material::ConfigureCombiners(PersistentData& storageData)
{
	//bool					mMultitexturingEnabled;
	//CombinerCommands		mCombinerCommands[MAX_TEXTURES_PER_MATERIAL];
	std::list<PersistentData*> textureBindings;
	storageData.GetNodesFromPath("TextureBinding",textureBindings);
	Renderer& rend = Game::GetInstance().GetRenderer();
	
	unsigned int textureUnit = 0;
	std::list<PersistentData*>::iterator textureBindingsIt = textureBindings.begin();
	while(textureBindingsIt != textureBindings.end())
	{
		
		PersistentData* tb = *textureBindingsIt;
		if(textureUnit >= MAX_TEXTURES_PER_MATERIAL)
			break;
		
		mMultiTexturingEnabled = true;
		
		CombinerCommands& cc = mCombinerCommands[textureUnit];
		//cc.mTextureUnit = textureUnit;
		//cc.mTextureUnitEnum =  rend.GetEnumForString(tb->GetProperty("Op"));
		
		
		std::list<PersistentData*> textureEnvCmds;
		tb->GetNodesFromPath("TextureEnv",textureEnvCmds);
		std::list<PersistentData*>::iterator txEnvIt = textureEnvCmds.begin();
		while(txEnvIt != textureEnvCmds.end())
		{
			unsigned int op0, op1;
			
			PersistentData* txEnvCmd = *txEnvIt;
			op0 = rend.GetEnumForString(txEnvCmd->GetProperty("Op1"));
			op1 = rend.GetEnumForString(txEnvCmd->GetProperty("Op2"));
			
			if(op0 == 0)
			{
				printf("ERROR!!!! There was an error while setting up TextureCombiners... couldn't translate op1 %s\n", txEnvCmd->GetProperty("Op1").c_str());
			}
			if(op1 == 0)
			{
				printf("ERROR!!!! There was an error while setting up TextureCombiners... couldn't translate op2 %s\n", txEnvCmd->GetProperty("Op2").c_str());
			}
					   
			cc.mCommands.push_back(std::pair<unsigned int, unsigned int>(op0,op1));
			
			++txEnvIt;
		}
		++textureUnit;
		++textureBindingsIt;
	}
	
}
////////////////////////////////////////////////////////////////////////////////
void Material::Serialize(PersistentData& storageData)
{
	storageData.SetName("Material");
	
	storageData.SetProperty("Name",mName.c_str());
	storageData.AddColorChild(mDiffuse,"Diffuse");
	storageData.AddColorChild(mSpecular,"Specular");
	storageData.AddColorChild(mAmbient,"Ambient");
	storageData.AddColorChild(mEmissive,"Emissive");
	//storageData.SetProperty("MainTexture",mMainTexture);
	storageData.SetProperty("TextureCount",mTextureCount);
	storageData.SetProperty("IsFont",mIsFont);
	storageData.SetProperty("GameID",mGameID);

	
	
	char buff[128];
	unsigned int objID;
	//GameID				mTextureIDs[MAX_TEXTURES_PER_MATERIAL];
	PersistentData* textures = new PersistentData("Textures");
	storageData.AddChild(textures);
	
	for(int i = 0; i < MAX_TEXTURES_PER_MATERIAL; ++i)
	{
		PersistentData* c = new PersistentData("Texture");
		objID = static_cast<unsigned int>(mTextureIDs[i]);
		sprintf(buff, "%d",objID);
		std::string s = buff;
		c->SetTextProperty(s);
		textures->AddChild(c);
	}

	
	
}
////////////////////////////////////////////////////////////////////////////////
void Material::DeSerialize(PersistentData& storageData)
{	
	mDiffuse = storageData.GetColorChild("Diffuse");
	mSpecular = storageData.GetColorChild("Specular");
	mAmbient = storageData.GetColorChild("Ambient");
	mEmissive = storageData.GetColorChild("Emissive");

	//mMainTexture = static_cast<GameID> (storageData.GetPropertyU("MainTexture"));
	mTextureCount = storageData.GetPropertyI("TextureCount");
	mIsFont = storageData.GetPropertyB("IsFont");
	mName = storageData.GetProperty("Name");
	mGameID = static_cast<GameID> (storageData.GetPropertyU("GameID"));
	
	std::list<PersistentData*> children;
	
	storageData.GetNodesFromPath("Textures",children);
	std::list<PersistentData*>::iterator it = children.begin();
	int i = 0;
	while(it != children.end() && i < MAX_TEXTURES_PER_MATERIAL)
	{
		PersistentData*c = (*it);
		GameID texID = static_cast<GameID> (c->GetTextAsUInt());
		mTextureIDs[i++] = texID;
		++it;
	}
	
	RendererObjectFactory::RegisterMaterial(mGameID, mName.c_str());
}


////////////////////////////////////////////////////////////////////////////////
void Material::AddTexture(GameID textureID)
{
	if(mTextureCount >= MAX_TEXTURES_PER_MATERIAL - 1)
		return;
	
	mTextureIDs[mTextureCount] = textureID;
	
	Renderer& r = Game::GetInstance().GetRenderer();
	r.Retain(mGameID,textureID);
	
	mTextureCount++;
	

}
////////////////////////////////////////////////////////////////////////////////
void Material::RemoveTexture(GameID textureID)
 {
	 std::vector<GameID> textures;
	 Renderer& r = Game::GetInstance().GetRenderer();
	 
	 for(int i = 0; i < mTextureCount; i++)
	 {
		 GameID id = mTextureIDs[i];
		 if(mTextureIDs[i] == textureID)
		 {
			 r.Release(mGameID,id);
		 }
		 else
		 {
			textures.push_back(id); 
		 }
		 mTextureIDs[i] = 0;
	 }
	 
	 mTextureCount = 0;
	 for(unsigned int j = 0; j < textures.size(); j++)
	 {
		 mTextureIDs[j] = textures[j];
		 mTextureCount++;
	 }
 
 
 }
////////////////////////////////////////////////////////////////////////////////
void Material::SetMainTexture(GameID textureID)
{	
	if(mTextureIDs[0] != 0)
	{
		Renderer& r = Game::GetInstance().GetRenderer();
		r.Release(mGameID, mTextureIDs[0]);
	}
	
	mTextureIDs[0] = textureID;
	if(mTextureCount == 0)
	{
		mTextureCount++;
	}
}
////////////////////////////////////////////////////////////////////////////////
void Material::ClearTextures()
{
	Renderer& r = Game::GetInstance().GetRenderer();
	for(int i = 0; i < mTextureCount; i++)
	{
		r.Release(mGameID,mTextureIDs[i]);
		mTextureIDs[i] = 0;
	}
	
	//memset(mTextureIDs, 0, sizeof(GameID)*MAX_TEXTURES_PER_MATERIAL);
	mTextureCount = 0;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrix(IvMatrix33& rot, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.Rotation(rot);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrix(IvQuat& rot, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.Rotation(rot);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrix(IvVector3& axis, float angle, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.Rotation(axis, angle);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrix(float x_angle, float y_angle, float z_angle, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.Rotation(x_angle, y_angle, z_angle);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrixX(float angle, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.RotationX(angle);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrixY(float angle, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.RotationY(angle);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::RotateTextureMatrixZ(float angle, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.RotationZ(angle);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::TranslateTextureMatrix(const IvVector3& p, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.Translation(p);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::ScaleTextureMatrix(const IvVector3& scale, int idx)
{
	//mUseTextureMatrix = true;
	IvMatrix44 m;
	m.Scaling(scale);
	mTextureMatrices[idx] *= m;
}
////////////////////////////////////////////////////////////////////////////////
void Material::SetupShaders()
{
	bool hasCompiler;
	int numFormats;
	int* formatsArray;
	Renderer& renderer = Game::GetInstance().GetRenderer();
	
	renderer.GetShaderCompilerInfo(&hasCompiler, &numFormats, formatsArray);
	free(formatsArray);
	
	if(hasCompiler == false){
		printf("ERROR!!!! Cannot compile shaders... driver lacks a compiler!!!!\n");
		return;
	}
	
	
	ReadAndCompileShaders();
	
	if(mShaderProgramID == 0)
	{
		return;
	}
	
	ConfigureAttributeBindings();
	ConfigureUniformBindings();
}

////////////////////////////////////////////////////////////////////////////////
void Material::ReadAndCompileShaders()
{
	Renderer& renderer = Game::GetInstance().GetRenderer();
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
	
	std::string line;
	
	//printf("Loading Vertex Shader %s\n",mVertexShaderPath.c_str());
	std::string fullpath;
	std::string file = mVertexShaderPath;
	GetFullFilePathFromResource(file,fullpath);
	std::ifstream textstream(fullpath.c_str());
	while (std::getline(textstream, line)) 
	{
		vertexShaderSource += line;
		vertexShaderSource += "\n";
	} 
	textstream.close();
	
	//printf("Loading Fragment Shader %s\n",mFragmentShaderPath.c_str());
	file = mVertexShaderPath;
	GetFullFilePathFromResource(file,fullpath);
	textstream.open(fullpath.c_str());
	while (std::getline(textstream, line)) 
	{
		fragmentShaderSource += line;
		fragmentShaderSource += "\n";
	} 
	textstream.close();
	
	mShaderProgramID = renderer.LoadProgram(vertexShaderSource.c_str(),fragmentShaderSource.c_str());
	
}
////////////////////////////////////////////////////////////////////////////////
void Material::ConfigureAttributeBindings()
{
	
	Renderer& renderer = Game::GetInstance().GetRenderer();
	std::list<ShaderAttributeBinding> bindingInfo;
	renderer.GetActiveAttributeNamesAndLocations(mShaderProgramID,bindingInfo);
	
	std::list<ShaderAttributeBinding>::iterator it = bindingInfo.begin();
	while(it != bindingInfo.end())
	{
		ShaderAttributeBinding sab = *it;
		if(sab.mAttributeName == "input_position")
		{
			mVertexPositionAttrLocation = sab.mLocationID;
		}
		else if(sab.mAttributeName == "input_normal")
		{
			mVertexNormalAttrLocation = sab.mLocationID;
		}
		else if(sab.mAttributeName == "input_color")
		{
			mVertexColorAttrLocation = sab.mLocationID;
		}
		else if(sab.mAttributeName == "input_texcoord" || sab.mAttributeName == "input_texcoord0")
		{
			mVertexTextureCoord0AttrLocation = sab.mLocationID;
		}
		else if(sab.mAttributeName == "input_texcoord1")
		{
			mVertexTextureCoord1AttrLocation = sab.mLocationID;
		}
		++it;
	}
}
////////////////////////////////////////////////////////////////////////////////

void Material::ConfigureUniformBindings()
{
	Renderer& renderer = Game::GetInstance().GetRenderer();
	std::list<ShaderUniformBinding> bindingInfo;
	renderer.GetActiveUniformNamesAndLocations(mShaderProgramID,bindingInfo);
	
	std::list<ShaderUniformBinding>::iterator it = bindingInfo.begin();
	while(it != bindingInfo.end())
	{
		ShaderUniformBinding sub = *it;
		if(sub.mUniformVariableName == "input_texture" || sub.mUniformVariableName == "input_texture0")
		{
			mTextureUniformLocations[0] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture1")
		{
			mTextureUniformLocations[1] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture2")
		{
			mTextureUniformLocations[2] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture3")
		{
			mTextureUniformLocations[3] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture4")
		{
			mTextureUniformLocations[4] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture5")
		{
			mTextureUniformLocations[5] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture6")
		{
			mTextureUniformLocations[6] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texture7")
		{
			mTextureUniformLocations[7] = sub.mLocationID;
		}
		
		
		if(sub.mUniformVariableName == "input_texturematrix" || sub.mUniformVariableName == "input_texturematrix0")
		{
			mTextureMatrixUniformLocations[0] = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_texturematrix1")
		{
			mTextureMatrixUniformLocations[1] = sub.mLocationID;
		}
		
		if(sub.mUniformVariableName == "input_modelviewmatrix")
		{
			mModelViewMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_modelviewinversematrix")
		{
			mModelViewInverseMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_modelviewtransposematrix")
		{
			mModelViewTransposeMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_modelviewinversetransposematrix")
		{
			mModelViewInverseTransposeMatrixUniformLocation = sub.mLocationID;
		}
		
		if(sub.mUniformVariableName == "input_modelviewprojectionmatrix")
		{
			mModelViewProjectionMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_modelviewprojectioninversematrix")
		{
			mModelViewProjectionInverseMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_modelviewprojectiontransposematrix")
		{
			mModelViewProjectionTransposeMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_modelviewprojectioninversetransposematrix")
		{
			mModelViewProjectionInverseTransposeMatrixUniformLocation = sub.mLocationID;
		}
		
		if(sub.mUniformVariableName == "input_projectionmatrix")
		{
			mProjectionMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_projectioninversematrix")
		{
			mProjectionInverseMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_projectiontransposematrix")
		{
			mProjectionTransposeMatrixUniformLocation = sub.mLocationID;
		}
		else if(sub.mUniformVariableName == "input_projectioninversetransposematrix")
		{
			mProjectionInverseTransposeMatrixUniformLocation = sub.mLocationID;
		}
		++it;
	}	
}
////////////////////////////////////////////////////////////////////////////////

/*
void Material::SetupUniformAndAttributeDBs()
{

	 mAttributeNameToAttributeSlotDB["VERTEX_POSITION"] = 0;
	mAttributeNameToAttributeSlotDB["VERTEX_NORMAL"] = 1;
	mAttributeNameToAttributeSlotDB["VERTEX_COLOR"] = 2;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD0"] = 3;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD1"] = 4;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD2"] = 5;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD3"] = 6;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD4"] = 7;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD5"] = 8;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD6"] = 9;
	mAttributeNameToAttributeSlotDB["VERTEX_TEXCOORD7"] = 10;
	
	ShaderUniformBinding nothing;
	mUniformNameToShaderSlotDB["TEXTURE0"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE1"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE2"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE3"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE4"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE5"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE6"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE7"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE0_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE1_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE2_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE3_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE4_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE5_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE6_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["TEXTURE7_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["MODELVIEW_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["MODELVIEWPROJECTION_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["PROJECTION_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["MODELVIEW_INVERSE_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["MODELVIEWPROJECTION_INVERSE_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["PROJECTION_INVERSE_MATRIX"] = nothing;
	
	mUniformNameToShaderSlotDB["MODELVIEW_TRANSPOSE_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["MODELVIEWPROJECTION_TRANSPOSE_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["PROJECTION_TRANSPOSE_MATRIX"] = nothing;
	
	mUniformNameToShaderSlotDB["MODELVIEW_INVERSE_TRANSPOSE_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["MODELVIEWPROJECTION_INVERSE_TRANSPOSE_MATRIX"] = nothing;
	mUniformNameToShaderSlotDB["PROJECTION_INVERSE_TRANSPOSE_MATRIX"] = nothing;
}
*/