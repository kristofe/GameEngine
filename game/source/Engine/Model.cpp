/*
 *  Model.cpp
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Model.h"
#include "Platform.h"
#include "Globals.h"
#include "Mesh.h"

#include <stdlib.h>
//#include <stdio.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
Model::Model(){
	mDisplayModel = NULL;
	mChangeBufferModel = NULL;
	mUpdatesArePending = false;
	Threading::MutexInit(mMutex);

}
////////////////////////////////////////////////////////////////////////////////
/*
Model::Model(Mesh* md){
	Model::Model();
	mDisplayModel = md;

}
////////////////////////////////////////////////////////////////////////////////

Model::~Model()
{
	SAFE_DELETE(mDisplayModel);
	SAFE_DELETE(mChangeBufferModel);
	Threading::MutexDestroy(mMutex);
}
////////////////////////////////////////////////////////////////////////////////

void Model::Update(Mesh* updatedModel){
	Threading::MutexLock(mMutex);
	mUpdatesArePending = true;
	mChangeBufferModel = updatedModel;
	Threading::MutexUnlock(mMutex);
	
};
////////////////////////////////////////////////////////////////////////////////

void Model::ForceSyncOfModelData(){
	Threading::MutexLock(mMutex);
	if(mUpdatesArePending){
		SyncModelData();
		mUpdatesArePending = false;
	}
	Threading::MutexUnlock(mMutex);
}

////////////////////////////////////////////////////////////////////////////////

void Model::SyncModelData(){
	if(mChangeBufferModel == NULL) return;
	Threading::MutexLock(mMutex);
	if(mDisplayModel == NULL){
		mDisplayModel = new Mesh();
	}
	
	mDisplayModel->Sync(mChangeBufferModel);
	mChangeBufferModel->SetDirtyFlags(MESH_STATE_CLEAN);
	mDisplayModel->SetDirtyFlags(MESH_STATE_CLEAN);
	
	Threading::MutexUnlock(mMutex);
	
}
////////////////////////////////////////////////////////////////////////////////

bool Model::HasPendingUpdates(){
	Threading::MutexLock(mMutex);
	return mUpdatesArePending;
	
	Threading::MutexUnlock(mMutex);
	
}
*/

