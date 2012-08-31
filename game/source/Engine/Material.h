#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <map>
#include "IvMathLib.h"
#include "Texture.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"
#include "Platform.h"


struct CombinerCommands
{
	std::vector< std::pair<unsigned int, unsigned int> > mCommands;
};

struct ShaderUniformBinding
{
	std::string   mUniformVariableName;
	int  mLocationID;
	unsigned int mGLType;
	
	ShaderUniformBinding():mUniformVariableName(""),mLocationID(-1),mGLType(0){};
	ShaderUniformBinding		&operator =  (const ShaderUniformBinding &m)
	{
		if(this == &m)
		{
			return *this;
		}
		mUniformVariableName = m.mUniformVariableName;
		mLocationID = m.mLocationID;
		mGLType = m.mGLType;
		return *this;
	};
	
	ShaderUniformBinding(const ShaderUniformBinding &m){
		mUniformVariableName = m.mUniformVariableName;
		mLocationID = m.mLocationID;
		mGLType = m.mGLType;
	};
};

struct ShaderAttributeBinding
{
	std::string   mAttributeName;
	int  mLocationID;
	unsigned int mGLType;
	
	ShaderAttributeBinding():mAttributeName(""),mLocationID(-1),mGLType(0){};
	ShaderAttributeBinding		&operator =  (const ShaderAttributeBinding &m)
	{
		if(this == &m)
		{
			return *this;
		}
		mAttributeName = m.mAttributeName;
		mLocationID = m.mLocationID;
		mGLType = m.mGLType;
		return *this;
	};
	
	ShaderAttributeBinding(const ShaderAttributeBinding &m){
		mAttributeName = m.mAttributeName;
		mLocationID = m.mLocationID;
		mGLType = m.mGLType;
	};
};

class Material{
	friend class Renderer;
	friend class RendererObjectFactory;
public:
	
	Material(std::string name):mName(name),mTextureCount(0),mIsFont(false)
	{
		mTextureCount = 0;
		memset(mTextureIDs, 0, sizeof(GameID)*MAX_TEXTURES_PER_MATERIAL);
		mDiffuse.Set(1,1,1,1);
		mSpecular.Set(0,0,0,0);
		mAmbient.Set(0, 0, 0, 0);
		mEmissive.Set(0,0,0,0);
		mZWrite = true;
		mZTest = true;
		mBlend = true;
		mBlendFuncA = (int)GL_SRC_ALPHA;
		mBlendFuncB = (int)GL_ONE_MINUS_SRC_ALPHA;
		mUseRenderTarget = false;
		mUseAlphaTest = false;
		mAlphaFunc = (int)GL_GREATER;
		mAlphaFuncTestValue = 0.5f;
		mMultiTexturingEnabled = false;
		mUseColorMask = false;
		mShaderProgramID = -1;
		
		mVertexPositionAttrLocation = -1;
		mVertexNormalAttrLocation = -1;
		mVertexColorAttrLocation = -1;
		mVertexTextureCoord0AttrLocation = -1;
		mVertexTextureCoord1AttrLocation = -1;
		
		for(int i = 0; i < 8; i++)
			mTextureUniformLocations[i] = -1;
		for(int i = 0; i < 2; i++)
			mTextureMatrixUniformLocations[i] = -1;
		
		mModelViewMatrixUniformLocation = -1;
		mModelViewInverseMatrixUniformLocation = -1;
		mModelViewTransposeMatrixUniformLocation = -1;
		mModelViewInverseTransposeMatrixUniformLocation = -1;
		
		mModelViewProjectionMatrixUniformLocation = -1;
		mModelViewProjectionInverseMatrixUniformLocation = -1;
		mModelViewProjectionTransposeMatrixUniformLocation = -1;
		mModelViewProjectionInverseTransposeMatrixUniformLocation = -1;
		
		mProjectionMatrixUniformLocation = -1;
		mProjectionInverseMatrixUniformLocation = -1;
		mProjectionTransposeMatrixUniformLocation = -1;
		mProjectionInverseTransposeMatrixUniformLocation = -1;
		//SetupUniformAndAttributeDBs();
	};
	~Material(){};
private:
	//Remove Copy Constructor and assignment operator
	Material		&operator =  (const Material &m);
	Material(const Material &m);
	
public:
	IvColor					mDiffuse;
	IvColor					mSpecular;
	IvColor					mAmbient;
	IvColor					mEmissive;
	IvColor					mColorMask;

	bool					mUseColorMask;
	bool					mUseRenderTarget;
	std::string				mRenderTargetTag;
	GameID					mRenderTargetID;
	
	//GameID				mMainTexture;
	//unsigned int			mMainTextureHWID;
	GameID					mTextureIDs[MAX_TEXTURES_PER_MATERIAL];
	unsigned int			mMainTextureHWIDs[MAX_TEXTURES_PER_MATERIAL];
	int						mTextureCount;

	bool					mIsFont;
	GameID					mGameID;
	std::string				mName;
	bool					mZWrite;
	bool					mZTest;
	bool					mBlend;
	int						mBlendFuncA;
	int						mBlendFuncB;
	bool					mUseAlphaTest;
	int						mAlphaFunc;
	float					mAlphaFuncTestValue;

	IvMatrix44				mTextureMatrices[MAX_TEXTURES_PER_MATERIAL];
	
	bool					mMultiTexturingEnabled;
	CombinerCommands		mCombinerCommands[MAX_TEXTURES_PER_MATERIAL];
	
	//Shader Support
	std::string		mVertexShaderPath;
	std::string		mFragmentShaderPath;
	unsigned int	mShaderProgramID;
	
	//Attributes
	int			mVertexPositionAttrLocation;
	int			mVertexNormalAttrLocation;
	int			mVertexColorAttrLocation;
	int			mVertexTextureCoord0AttrLocation;
	int			mVertexTextureCoord1AttrLocation;
	
	//Uniforms
	int			mTextureUniformLocations[8];
	/*
	int			mTexture0UniformLocation;
	int			mTexture1UniformLocation;
	int			mTexture2UniformLocation;
	int			mTexture3UniformLocation;
	int			mTexture4UniformLocation;
	int			mTexture5UniformLocation;
	int			mTexture6UniformLocation;
	int			mTexture7UniformLocation;
	*/
	
	int			mTextureMatrixUniformLocations[2];
	//int			mTextureMatrix1UniformLocation;
	
	int			mModelViewMatrixUniformLocation;
	int			mModelViewInverseMatrixUniformLocation;
	int			mModelViewTransposeMatrixUniformLocation;
	int			mModelViewInverseTransposeMatrixUniformLocation;
	
	int			mModelViewProjectionMatrixUniformLocation;
	int			mModelViewProjectionInverseMatrixUniformLocation;
	int			mModelViewProjectionTransposeMatrixUniformLocation;
	int			mModelViewProjectionInverseTransposeMatrixUniformLocation;
	
	int			mProjectionMatrixUniformLocation;
	int			mProjectionInverseMatrixUniformLocation;
	int			mProjectionTransposeMatrixUniformLocation;
	int			mProjectionInverseTransposeMatrixUniformLocation;
	//Attribute mapping
	//NOT NECESSARY - 
	//constructed by code ... translates VERTEX_POSITION into a slot... 
	//allows for "a_position" = VERTEX_POSITION which used in glBindAttributeLocation(progamID,0{VERTEX_POSITION},"a_position");
	//std::map<std::string, unsigned int>  mAttributeNameToAttributeSlotDB; 
	
	//Example ("a_position",0) if in config it was VERTEX_POSITION=>"a_position"
	//XML <ATTRIBUTE type="VERTEX_POSITION" name="a_position"/> -- NO XML NEEDED IF A NAMING CONVENTION IS ADOPTED
	//The binding is set when we load the xml and then just used when loading material in the draw loop
	//std::map<std::string, unsigned int> mAttributeBindings; 
	
	//Uniforms
	//constructed by glsl compiler... a list of names and their slots.
	//Example texture0 = 0;
	//so we need to say TEXTURE0 is called "s_texture" and it was assigned location 0
	//or MODELVIEW_MATRIX is called mvp_matrix and is assigned location 1
	//XML <UNIFORM reference="TEXTURE0" name="s_texture"/>
	//the binding has to be found by using mUniformNameToShaderSlotDB["TEXTURE0"].mLocationID = glGetUniformLocation(progamID, mUniformNameToShaderSlotDB["TEXTURE0"].mUniformVariableName);
	//Then at draw time glUniform*(mUniformNameToShaderSlotDB["TEXTURE0"].mLocationID, 0); 0 is there because the texture is bound to unit 0;//peculiar to textures
	//Another example is glUniformMatrix4fv ( mUniformNameToShaderSlotDB["PROJECTION_MATRIX"].mLocationID, 1, mProjectionMatrix );
	//std::map<std::string, ShaderUniformBinding>   mUniformNameToShaderSlotDB;
	//std::map<std::string, unsigned int> mUniformBindings;
	
public:
	//void AddMesh(Mesh* m);
	//void RemoveMesh(Mesh* m);
	void AddTexture(GameID textureID);
	void RemoveTexture(GameID textureID);
	void SetMainTexture(GameID textureID);
	void ClearTextures();
	GameID GetID(){return mGameID;};
	void Configure(PersistentData& storageData);
	void ConfigureCombiners(PersistentData& storageData);
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);

	void				RotateTextureMatrix(IvMatrix33& rot, int idx = 0);
	void				RotateTextureMatrix(IvQuat& rot, int idx = 0);
	void				RotateTextureMatrix(IvVector3& axis, float angle, int idx = 0);
	void				RotateTextureMatrix(float x_angle, float y_angle, float z_angle, int idx = 0);
	void				RotateTextureMatrixX(float angle, int idx = 0);
	void				RotateTextureMatrixY(float angle, int idx = 0);
	void				RotateTextureMatrixZ(float angle, int idx = 0);
	void				TranslateTextureMatrix(const IvVector3& p, int idx = 0);	
	void				ScaleTextureMatrix(const IvVector3& scale, int idx = 0);
	void				ResetTextureMatrix(int idx = 0) { mTextureMatrices[idx].Identity(); /*mUseTextureMatrix = false;*/};
	//void				SetUseTextureMatrix(bool v) { mUseTextureMatrix = v;};
	
	void SetupShaders();
	void ReadAndCompileShaders();
	void ConfigureAttributeBindings();
	void ConfigureUniformBindings();
	
	virtual std::string GetTypeString()
	{
		return "Material";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "%d\tMaterial\tName:%s",mGameID,mName.c_str());
		std::string s = buf;
		return s;
	}
	
};


#endif //_Geometry_H