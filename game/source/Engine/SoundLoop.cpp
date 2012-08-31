#include "SoundLoop.h"
#include "Platform.h"

SoundLoop::SoundLoop(char* loopFilename, char* startFilename, char* stopFilename)
{
	playing = false;
	std::string loopPath;
	std::string file = loopFilename;
	GetFullFilePathFromResource(file,loopPath);
	id = loadSoundLoop(loopPath.c_str(), NULL, NULL);
}

void SoundLoop::play(float volume, float pitch)
{
	if (playing) return;
	setSoundVolume(id, volume);
	setSoundPitch(id, pitch);
	playSound(id);
	playing = true;
}

void SoundLoop::stop(bool decay)
{
	if (!playing) return;
	stopSound(id, decay);
	playing = false;
}

void SoundLoop::setVolume(float volume)
{
	setSoundVolume(id, volume);
}