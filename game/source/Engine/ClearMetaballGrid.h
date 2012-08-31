#ifndef _CLEAR_METABALL_GRID_H
#define _CLEAR_METABALL_GRID_H

#include "GameObject.h"


class ClearMetaballGrid : public GameObject
	{
	protected:
		bool    mFinished;
	public:
		ClearMetaballGrid(){};
		ClearMetaballGrid(PersistentData& storageData);
		~ClearMetaballGrid();
		
		void Update();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "ClearMetaballGrid";
		}
	private:
		ClearMetaballGrid &operator=(const ClearMetaballGrid& m);
		ClearMetaballGrid(const ClearMetaballGrid&m);
		
	};
#endif //_DISABLE_INPUT_H

