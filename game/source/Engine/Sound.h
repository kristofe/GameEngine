#include "GWArray.h"


class Sound
{
	int nextId;
	GWArray<unsigned long> ids;
public:
	Sound(char* filename, int maxSimultaneous = 1);
	void play(float volume = 1.0f, float pitch = 1.0f);
	void stop(bool decay = false);
	void setVolume(float volume);
};