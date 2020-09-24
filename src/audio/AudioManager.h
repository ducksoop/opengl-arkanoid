#pragma once

#include "Singleton.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <map>
#include <string>

class AudioManager : public Singleton<AudioManager>
{
public:
	void Initialize();
	void Close();

	void CreateAudioSource(const std::string& name, const std::string& path, bool isLooping);
	void PlayAudioSource(const std::string& name);
	
private:
	ALCdevice* m_device;
	ALCcontext* m_context;

	std::map<std::string, std::pair<ALuint, ALuint>> m_sources;
};
