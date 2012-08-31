#ifndef _GAME_OBJECT_FACTORY_H
#define _GAME_OBJECT_FACTORY_H

class GameObject;
class PersistentData;

class GameObjectFactory
{
public:
	
	static GameObject* CreateGameObject(PersistentData& storageData);


};
#endif