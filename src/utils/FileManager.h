#pragma once

#include "Singleton.h"
#include "AudioFile.h"

#include <GL/glew.h>

#include <string>

class FileManager : public Singleton<FileManager>
{
public:
	std::string ReadAsText(const std::string& path) const;
	unsigned char* ReadImage(const std::string& path,
	                         GLint width,
	                         GLint height,
	                         GLint channels,
	                         bool flip = false) const;
	AudioFile ReadOggFile(const std::string& path) const;
	
private:
	FileManager() = default;
	~FileManager() = default;

	friend Singleton;
};
