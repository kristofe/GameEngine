/*
 *  RenderObject.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 2/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDER_OBJECT_H
#define _RENDER_OBJECT_H

#include <vector> 
#include "SceneObject.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"



//THIS CLASS SHOULD BE RENDER THREAD SAFE!!!!


class RenderObject : public SceneObject{
	friend class RendererObjectFactory;
	friend class Renderer;
	friend class RendererGL;
	friend class RendererGLES20;
	
protected:
	IvColor							mColor;
	std::vector<GameID>				mMeshes;
	std::vector<GameID>				mMaterials;
	bool							mIsTrasparent;
	bool							mVisible;
	//bool							mExclusiveToRenderTarget;
	//bool							mDisableZWrite;

	
	

	
public:
	RenderObject();
	RenderObject(PersistentData& storageData);
	
	~RenderObject();
	
	bool IsTransparent(){return mIsTrasparent;};
	bool IsVisible(){return mVisible;};
	void SetVisible(bool v){ mVisible = v;};
	void SetColor(const IvColor&);
	void SetUVs(const std::vector<IvVector2>& uvs);
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	//bool ExclusiveToRenderTarget(){return mExclusiveToRenderTarget;};
	//void SetExclusiveToRenderTarget(bool v){mExclusiveToRenderTarget = v;};
	
	virtual std::string GetTypeString()
	{
		return "RenderObject";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "\tMeshCount:%d\tMaterialCount:%d",(int)mMeshes.size(),(int)mMaterials.size());
		std::string superInfo = SceneObject::DebugOutput();
		std::string s = superInfo + buf;
		return s;
	}
	

protected:
	GameID ReuseMesh(const char* name){return (GameID)0;};
	void ReuseMesh(GameID meshID){};
	GameID ReuseMaterial(const char* name){return (GameID)0;};
	void ReuseMaterial(GameID matID){};

	
private:
	//Remove Copy Constructor and assignment operator
	RenderObject		&operator =  (const SceneObject &m) { printf("SceneObject = THIS ASSIGNMENT OPERATOR SHOULDN'T BE GETTING CALLED"); return *this;};
	RenderObject(const RenderObject &m){printf("SceneObject COPY CTOR SHOULDN'T BE GETTING CALLED");};;

public:
	static bool ZSortFunction(RenderObject* s1, RenderObject* s2)
	{
		return (s1->GetCameraDistance() < s2->GetCameraDistance());
	};	
	
};

#endif //_RENDER_OBJECT_H