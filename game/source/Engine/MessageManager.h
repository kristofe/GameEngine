#ifndef _MESSAGE_MANAGER_H
#define _MESSAGE_MANAGER_H

#include "GameObject.h"
#include <map>
#include <vector>
#include <string>


class Game;

class GameMessage{
public:
	GameID	mSender;
	bool			mBroadcast;
	std::string		mEventName;

	GameMessage(){
		mSender = 0;
		mBroadcast = false;
		mEventName = "DEFAULT";
	};
};

class MessageManager
{
protected:
	std::multimap<GameID, GameID>	mListenerDB;
	Game&										mGame;
public:
	MessageManager(Game& game);
	~MessageManager();

	void Serialize(PersistentData& storageData);
	void DeSerialize(PersistentData& storageData);
/*
	void AddListener(GameID target, GameID listener);
	void RemoveListener(GameID target, GameID listener);
	void RouteMessage(GameMessage& msg);
*/

private:
	MessageManager &operator=(const MessageManager& m);
	MessageManager(const MessageManager&m);
};
#endif