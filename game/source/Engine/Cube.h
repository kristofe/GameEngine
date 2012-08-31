#ifndef _GAME_ENGINE_CUBE_H
#define _GAME_ENGINE_CUBE_H

#include "RenderObject.h"

class Cube : public RenderObject
{
public:
	Cube();
	Cube(PersistentData& storageData);
	virtual ~Cube();
	
	void Init();
	
	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
	
	virtual std::string GetTypeString()
	{
		return "Cube";
	}

};

#endif