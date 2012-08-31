
#include "Sound.h"
#include "Platform.h"

Sound::Sound(char* filename, int maxSimultaneous)
{
	ids.ensureCapacity(maxSimultaneous);
	ids.size = maxSimultaneous;
	std::string path;
	std::string file = filename;
	GetFullFilePathFromResource(file,path);
	for (int i = 0; i < ids.size; i++)
		ids[i] = loadSound(path.c_str());
	nextId = 0;
}

void Sound::play(float volume, float pitch)
{
	setSoundVolume(ids[nextId], volume);
	setSoundPitch(ids[nextId], pitch);
	playSound(ids[nextId]);
	nextId = (nextId + 1)%ids.size;
}

void Sound::stop(bool decay)
{
	for (int i = 0; i < ids.size; i++)
		::stopSound(ids[i], decay);
}

void Sound::setVolume(float volume)
{
	for (int i = 0; i < ids.size; i++)
		::setSoundVolume(ids[i], volume);
}