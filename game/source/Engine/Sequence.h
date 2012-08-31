#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include "GameObject.h"

struct SequenceItemMetaData
{
	bool				mIsLinked;
	std::string			mGameObjectTag;
	bool				mDeactivate;
	std::string			mObjectType;
	PersistentData*		mStorageData;
};

class Sequence : public GameObject
{
protected:
	std::vector<SequenceItemMetaData*>		mSequenceItems;
	bool									mRepeat;
	int										mCurrentItemIndex;
	SequenceItemMetaData*					mCurrentItem;
	GameObject*								mCurrentGameObject;
	std::string								mMessageOnFinish;
	int										mPendingIndex;
	bool									mDone;

public:
	Sequence();
	Sequence(PersistentData& storageData);
	~Sequence();

	void Init();
	virtual void ProcessMessage(GameMessage& msg);
	virtual void StartAsSequenceItem(PersistentData* storageData = NULL);
	void StartItem(int index);

	virtual void Serialize(PersistentData& storageData);
	virtual void DeSerialize(PersistentData& storageData);
	virtual void Update();
	
	virtual std::string GetTypeString()
	{
		return "Sequence";
	}
	
	virtual std::string DebugOutput()
	{
		char buf[256];
		sprintf(buf, "\tMessageOnFinish:%s",mMessageOnFinish.c_str());
		std::string superInfo = GameObject::DebugOutput();
		std::string s = superInfo + buf;
		return s;
	}
};
#endif