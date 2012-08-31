%module example

%{
#include "Game.h"
#include "SimBase.h"
%}
class Game : public SimBase
{
public:
	static void TestLuaCallback();
};
