/*
 *  RendererGL.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDERER_H
#define _RENDERER_H

#include <list>
#include <set>
#include <map>
#include <list>
#include "Matrix.h"
#include "Thread.h"
#include "GameObject.h"


class Mesh;
class Model;
class Material;
class Texture;
class TextureFont;
class RenderObject;
class RenderTarget;
class Camera;
struct ShaderAttributeBinding;
struct ShaderUniformBinding;

//THIS CLASS SHOULD BE RENDER THREAD SAFE!!!!
class Game;
class RendererObjectFactory;

//typedef unsigned int GameID;


struct RendererObjectRefCounter
{
	GameID	rendererID;
	std::set<GameID> referencingObjects;
	
	void Serialize(PersistentData& storageData)
	{
		PersistentData* node = new PersistentData("RefCounter");
		node->SetProperty("GameID",rendererID);
		std::set<GameID>::iterator roIt = referencingObjects.begin();
		while (roIt != referencingObjects.end()) {
			PersistentData* c = new PersistentData("ReferencingGameObject");
			c->SetProperty("GameID",*roIt);
			node->AddChild(c);
			++roIt;
		}
		storageData.AddChild(node);
		
	};
	
	void DeSerialize(PersistentData& storageData)
	{
		rendererID	= static_cast<GameID> (storageData.GetPropertyU("GameID"));
		std::list<PersistentData*> children;
		storageData.GetNodesFromPath("RefCounter",children);
		std::list<PersistentData*>::iterator it = children.begin();
		while(it != children.end())
		{
			PersistentData* c = *it;
			GameID id = static_cast<GameID> (c->GetPropertyU("GameID"));
			referencingObjects.insert(id);
			++it;
		}
		
	};
	
	std::string DebugOutput()
	{
		char buf[512];
		std::string objList = "";
		std::set<GameID>::iterator roIt = referencingObjects.begin();
		while (roIt != referencingObjects.end()) {
			char b[32];
			sprintf(b,"%d",*roIt);
			objList += b;
			++roIt;
			if(roIt != referencingObjects.end())
				objList += ",";
		}
		sprintf(buf, "RefCounter\tGameID:%d\tReferencingObjects:%s",(int)rendererID,objList.c_str());
		std::string s =  buf;
		return s;
	}
};

typedef std::set<GameID>					RenderObjectSet;
typedef RenderObjectSet::iterator				RenderObjectSetIt;

typedef std::vector<RenderTarget*>				RenderTargetVector;
typedef RenderTargetVector::iterator			RenderTargetVectorIt;

typedef std::vector<RenderObject*>				RenderObjectVector;
typedef RenderObjectVector::iterator			RenderObjectVectorIt;

typedef std::map<GameID,RendererObjectRefCounter>	ReferenceCounterDB;
typedef ReferenceCounterDB::iterator					ReferenceCounterDBIt;

typedef std::map<GameID,Material*>			MaterialObjectMap;
typedef MaterialObjectMap::iterator				MaterialObjectMapIt;

typedef std::map<GameID,Mesh*>				MeshObjectMap;
typedef MeshObjectMap::iterator					MeshObjectMapIt;

typedef std::map<GameID,Texture*>			TextureObjectMap;
typedef TextureObjectMap::iterator				TextureObjectMapIt;

typedef std::map<GameID,TextureFont*>		TextureFontMap;
typedef TextureFontMap::iterator				TextureFontMapIt;


class Renderer
{
	friend class RendererObjectFactory;
public:
	enum {
		OpaqueObjectsArray = 0,
		TransparentObjectsArray = 1,
		ObjectsArrayCount = 2
	};
	public:
		Renderer(int pWidth, int pHeight, unsigned int frameBufferID=0, unsigned int renderBufferID=0):mFrameBufferID(frameBufferID),mRenderBufferID(renderBufferID),mT0(0),mFrames(0),mWidth(pWidth),mHeight(pHeight){
				//mLastGameID=0;
				mObjectVectors[OpaqueObjectsArray] = & mOpaqueRenderObjects;
				mObjectVectors[TransparentObjectsArray] = & mTranparentRenderObjects;
			mMainCamera = NULL;
				
		};
		virtual ~Renderer();
		
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		//virtual void RasterizeRenderTargets() = 0;
		virtual void Render() = 0;
		virtual void UseCamera(Camera* c) = 0;
		virtual void SetupStringToEnumDB() = 0;
		virtual unsigned int GetEnumForString(std::string str);
		virtual void SetGameEngine(Game* engine) { mGameEngine = engine;};
		virtual int	 GetWindowWidth(){return mWidth;};
		virtual int	 GetWindowHeight(){return mHeight;};
		virtual float GetFPS(){return mFPS;};
		virtual unsigned int GetFrameBufferID(){return mFrameBufferID;};
		virtual unsigned int GetRenderBufferID(){return mRenderBufferID;};

		virtual void SetupGPUState(Mesh& mesh,Material& mat)=0;
		virtual void RestoreGPUState(Mesh& mesh,Material& mat)=0;
		virtual void UploadTextureDataToGPU(Texture& tex)=0;
		virtual void DeleteTextureFromGPU(Texture& tex)=0;

		virtual void AddRenderObject(GameID renderObjectID);
		virtual void RemoveRenderObject(GameID renderObjectID);
	
		virtual void AddCamera(Camera* cam);
		virtual void RemoveCamera(Camera* cam);
		virtual void SetMainCamera(Camera* cam){mMainCamera = cam;};
		virtual Camera* GetMainCamera(){return mMainCamera;};
	
		virtual void AddRenderTarget(RenderTarget* renderTarget);
		virtual void RemoveRenderTarget(RenderTarget* renderTarget);

		virtual void Retain(GameID referencer, GameID referencee);
		virtual void Release(GameID referencer, GameID referencee);

		virtual GameID AddMesh(Mesh* m);
		virtual void DeleteMesh(GameID id);
		virtual Mesh& GetMesh(GameID id);
		virtual Mesh& CreateMesh(std::string name);
		virtual Mesh& LoadMesh(std::string name,std::string filename);

		virtual GameID AddMaterial(Material* m);
		virtual void DeleteMaterial(GameID id);
		virtual Material& GetMaterial(GameID id);
		virtual Material& CreateMaterial(std::string name);
		virtual Material& LoadMaterial(std::string name,std::string filename);

		virtual GameID AddTexture(Texture* m);
		virtual void DeleteTexture(GameID id);
		virtual Texture& GetTexture(GameID id);
		virtual Texture& CreateTexture(std::string name,std::string filename,bool delayLoad = false);
		virtual Texture& LoadTexture(std::string filename, bool delayLoad = false);

		virtual GameID AddTextureFont(TextureFont* m);
		virtual void DeleteTextureFont(GameID id);
		virtual TextureFont& GetTextureFont(GameID id);
		virtual TextureFont& CreateTextureFont(std::string name);
	
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
	
	
		bool IsShaderCompilerAvailable(){return false;};
		unsigned int LoadShader ( unsigned int type, const char *shaderSrc ){return 0;};
		unsigned int LoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc ){return 0;};
		void GetActiveAttributeNamesAndLocations(unsigned int programObj, std::list<ShaderAttributeBinding>&  metaDataList){};
		void GetActiveUniformNamesAndLocations(unsigned int programObj, std::list<ShaderUniformBinding>&  metaDataList){};
		bool GetShaderCompilerInfo(bool hasCompiler, int* numBinaryFormats, int* formats){return false;};
	
		virtual std::string GetTypeString()
		{
			return "Renderer";
		}
	
		virtual void DumpDebugData();
		
	protected:
		virtual void HandlePendingRendererObjectDeletions();
		virtual void HandlePendingRenderObjectRemovals();
		virtual void HandlePendingRenderTargetRemovals();
		virtual void HandlePendingCameraRemovals();
	
		virtual void CreateFrameBufferObject(RenderTarget& rt)=0;
		virtual int GetCurrrentFrameBufferObjectID()=0;
		virtual int GetCurrrentRenderBufferObjectID()=0;
		virtual void BindFrameBuffer(int renderBufferID)=0;
		virtual void BindRenderBuffer(int renderBufferID)=0;
	private:
		virtual void DeleteRendererObject(GameID id);
		

	protected:
		int mT0;
		int mFrames;
		float mFPS;
		int mWidth;
		int mHeight;
		int							mMaxTextureUnits;
		bool						mColorArrayActive;
		bool						mVertexArrayActive;
		bool						mTextureCoordArrayActive;
		bool						mTexturingEnabled[MAX_TEXTURES_PER_MATERIAL];
		bool						mZTestEnabled;
		bool						mZWriteEnabled;
		bool						mBlendEnabled;
		bool						mAlphaTestEnabled;
		bool						mMultiTexturingEnabled;
		IvColor						mColorMask;
		int							mBlendFuncA;
		int							mBlendFuncB;
		int							mAlphaFunc;
		float						mAlphaFuncValue;
		int							mBoundTexture[MAX_TEXTURES_PER_MATERIAL];
		unsigned int				mFrameBufferID;
		unsigned int				mRenderBufferID;
		//GameID					mLastLoadedMaterialID;
		//GameID					mLastLoadedMeshID;
		Game*						mGameEngine;
		Camera*						mMainCamera;
		RenderObjectSet				mRenderObjects;
		RenderTargetVector			mRenderTargets;						//Don't serialize
		RenderObjectVector			mOpaqueRenderObjects;				//Don't serialize
		RenderObjectVector			mTranparentRenderObjects;			//Don't serialize
		RenderObjectVector*			mObjectVectors[ObjectsArrayCount];	//Don't serialize
		ReferenceCounterDB			mRendererRefCountDB;
		
		std::map<std::string, unsigned int>		mStringToEnumDB;
	
		//GameID					mLastGameID;

		MaterialObjectMap			mMaterials;
		MeshObjectMap				mMeshes;
		TextureObjectMap			mTextures;
		TextureFontMap				mFonts;
		std::vector<Camera*>		mCameras;

		std::list<GameID>			mPendingRendererObjectDeletions;
		std::list<GameID>			mPendingRenderObjectRemovals;
		std::list<RenderObject*>	mPendingRenderObjectRemovalPointers;
	
		//std::list<GameID>			mPendingRendererTargetDeletions;
		std::list<RenderTarget*>	mPendingRenderTargetRemovals;
		std::list<Camera*>			mPendingCameraRemovals;
		//std::list<GameObject*>	mPendingRenderTargetRemovalPointers;
};

#endif //_RENDERER_H