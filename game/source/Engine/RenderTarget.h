#ifndef _RENDER_TARGET_H
#define _RENDER_TARGET_H

#include "SceneObject.h"
#include "Renderer.h"

class Game;
class GameLogic;
class LetterChooser;
class WordProgressDisplay;
class HangedManDisplay;
class Sequence;
class MetaballGrid;
class TextureFont;
class LetterDisplayAnimation;

class RenderTarget : public SceneObject
	{
	public:
		unsigned int				mFBOID;
		unsigned int				mFBO_DEPTHID;
		Texture*					mTargetTexture;
		GameID						mTextureID;
	protected:

		//bool						mRenderEverything;
		//std::vector<std::string>	mRenderObjectTags;
		//std::vector<bool>			mExclusiveToRenderTarget;
		//std::list<GameID>			mRenderObjectIDs;
		unsigned int				mWidth;
		unsigned int				mHeight;
		unsigned int				mOffsetX;
		unsigned int				mOffsetY;
		bool						mHasAlpha;
		
	public:
		RenderTarget(){};
		RenderTarget(PersistentData& storageData);
		~RenderTarget();
		
		void LevelLoaded();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		//void AddRenderObject(RenderObject* ro);
		//void RemoveRenderObject(RenderObject* ro);
		
		//bool RenderEverything(){return mRenderEverything;};
		//void GetObjectVectorsPtr(){return & mObjectVectors;};
		inline const unsigned int GetWidth(){return mWidth;};
		inline const unsigned int GetHeight(){return mHeight;};
		inline const unsigned int GetOffsetX(){return mOffsetX;};
		inline const unsigned int GetOffsetY(){return mOffsetY;};
		inline const GameID	GetTextureID() {return mTextureID;};
		inline Texture& GetTexture(){return *mTargetTexture;};
		//inline const unsigned int GetTextureHardwareID() {if(mTargetTexture != NULL) return mTargetTexture->mHardwareID; else return 0;};
		
		virtual std::string GetTypeString()
		{
			return "RenderTarget";
		}
		
		/*virtual std::string DebugOutput()
		{
			char buf[256];
			sprintf(buf, "\tRenderObjectIDCount:%d",mRenderObjectIDs.size());
			std::string superInfo = SceneObject::DebugOutput();
			std::string s = superInfo + buf;
			return s;
		}*/
		
		//RenderObjectVector			mOpaqueRenderObjects;				//Don't serialize
		//RenderObjectVector			mTranparentRenderObjects;			//Don't serialize
		//RenderObjectVector*			mObjectVectors[Renderer::ObjectsArrayCount];	//Don't serialize
	private:
		RenderTarget &operator=(const GameLogic& m);
		RenderTarget(const GameLogic&m);
		
	};
#endif //_SEQUENCE_DELAYER_H