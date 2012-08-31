#include <stdlib.h>

class SoundLoop
{
	unsigned long id;
	bool playing;
public:
	SoundLoop(char* loopFilename, char* startFilename = NULL, char* stopFilename = NULL);
	void play(float volume = 1.0f, float pitch = 1.0f);
	void stop(bool decay = false);
	void setVolume(float volume);
};