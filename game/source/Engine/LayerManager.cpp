/*
 *  LayerManager.cpp
 *  Mac
 *
 *  Created by Kristofer Schlachter on 11/29/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "LayerManager.h"
#include "PersistentDataManager.h"

////////////////////////////////////////////////////////////////////////////////
LayerManager*					LayerManager::mInstance = NULL;
////////////////////////////////////////////////////////////////////////////////
LayerManager& LayerManager::GetInstance()
{
	return *mInstance;
}
////////////////////////////////////////////////////////////////////////////////
LayerManager::LayerManager(PersistentData& storageData)
{
	std::list<PersistentData*>& layers = storageData.GetChildren();
	std::list<PersistentData*>::iterator it = layers.begin();
	while(it != layers.end())
	{
		std::string name = (*it)->GetProperty("Name");
		int id = (*it)->GetPropertyI("value");
		mLayerIDs[name] = id;
		++it;
	}
	
	mInstance = this;
	
}
////////////////////////////////////////////////////////////////////////////////
int LayerManager::GetLayerID(std::string name)
{
	int ret = 0;
	std::map<std::string, int>::iterator it = mLayerIDs.find(name);
	if(it != mLayerIDs.end())
		ret = it->second;

	return ret;
	
}
////////////////////////////////////////////////////////////////////////////////
LayerManager::~LayerManager()
{
	mLayerIDs.clear();
}