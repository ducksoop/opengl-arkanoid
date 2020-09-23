#include "AudioManager.h"
#include "AudioFile.h"
#include "FileManager.h"

#include <iostream>

void AudioManager::Initialize()
{
	m_device = alcOpenDevice(nullptr);
	if (!m_device)
	{
		std::cerr << "Cannot open sound device.\n";
	}

	m_context = alcCreateContext(m_device, nullptr);
	alcMakeContextCurrent(m_context);
}

void AudioManager::Close()
{
	for (auto& source : m_sources)
	{
		alDeleteSources(1, &source.second.first);
		alDeleteSources(1, &source.second.second);
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}

void AudioManager::CreateAudioSource(const std::string& name, const std::string& path, bool isLooping)
{
	AudioFile audioFile = FileManager::ReadOggFile(path);
	ALuint source, buffer;

	alGenSources(1, &source);
	alGenBuffers(1, &buffer);

	alBufferData(buffer, audioFile.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
		audioFile.data.get(), audioFile.GetSampleCount() * sizeof(ALshort), audioFile.sampleRate);

	alSourcei(source, AL_BUFFER, buffer);
	alSourcei(source, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);

	m_sources[name] = std::make_pair(source, buffer);
}

void AudioManager::PlayAudioSource(const std::string& name)
{
	ALuint source = m_sources[name].first;
	alSourcePlay(source);
}
