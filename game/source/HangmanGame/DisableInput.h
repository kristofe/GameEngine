#ifndef _DISABLE_INPUT_H
#define _DISABLE_INPUT_H

#include "GameObject.h"


class DisableInput : public GameObject
{
	protected:
		bool	mValue;
		bool    mFinished;
	public:
		DisableInput(){};
		DisableInput(PersistentData& storageData);
		~DisableInput();
		
		void Update();
		void StartAsSequenceItem(PersistentData* storageData = NULL);
		void StopAsSequenceItem();
		
		void Serialize(PersistentData& storageData);
		void DeSerialize(PersistentData& storageData);
		
		virtual std::string GetTypeString()
		{
			return "DisableInput";
		}
	private:
		DisableInput &operator=(const DisableInput& m);
		DisableInput(const DisableInput&m);
		
};
#endif //_DISABLE_INPUT_H

