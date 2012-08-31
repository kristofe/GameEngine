#ifndef _LAYER_MANAGER_H
#define _LAYER_MANAGER_H

#include <map>
#include "GameObject.h"
#include "PersistentDataManager.h"

class LayerManager
{
	protected:
		static LayerManager*				mInstance;
		std::map<std::string, int>			mLayerIDs;
	public:
		static LayerManager& GetInstance();
		LayerManager(){};
		LayerManager(PersistentData& storageData);
		~LayerManager();
	
		int GetLayerID(std::string name);
	private:
		LayerManager &operator=(const LayerManager& m);
		LayerManager(const LayerManager&m);
};
#endif

