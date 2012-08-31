/*
 *  Model.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MODEL_H
#define _MODEL_H

#include "Matrix.h"
#include "Mesh.h"
#include "Model.h"
#include "Thread.h"
#include "Renderer.h"
#include "RendererObjectFactory.h"

//THIS CLASS CAN BE MADE THREAD SAFE

class Renderer;

class Model
	{
		friend class Renderer;
		friend class RendererObjectFactory;
		
	public:
		Model();
		Model(Mesh* meshData);
		virtual ~Model();
		
		//bool HasPendingUpdates();
		//void Update(Mesh* meshData);
		//void ForceSyncOfModelData();
		
		
	private:
		//void SyncModelData();
		
	protected:
		GameID			mRendererMesh;
		//Mesh*				mMutableMesh;
		//Mutex*				mMutex;

		//RendererModelID		mGameID;

};



#endif //MODEL_H