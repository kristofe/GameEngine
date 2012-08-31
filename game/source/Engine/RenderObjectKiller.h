#ifndef _RENDER_OBJECT_KILLER_H
#define _RENDER_OBJECT_KILLER_H

#include "GameObject.h"


class RenderObjectKiller : public GameObject
	{
	protected:
		std::string		mGameObjectTag;
		bool    mDeactivateOnly;
	public:
		RenderObjectKiller(){};
		RenderObjectKiller(PersistentData& storageData);
		~RenderObjectKiller();
		
		void Update();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "RenderObjectKiller";
		}
		
	private:
		RenderObjectKiller &operator=(const RenderObjectKiller& m);
		RenderObjectKiller(const RenderObjectKiller&m);
		
	};
#endif //_DISABLE_INPUT_H
