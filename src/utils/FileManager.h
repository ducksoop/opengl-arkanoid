#pragma once

#include "Singleton.h"
#include "AudioFile.h"

#include <GL/glew.h>

#include <string>
#include <vector>

class FileManager : public Singleton<FileManager>
{
public:
	static std::string ReadAsText(const std::string& path);
	static std::vector<unsigned char> ReadAsBinary(const std::string& path);
	static unsigned char* ReadImage(const std::string& path,
	                         GLint width,
	                         GLint height,
	                         GLint channels,
	                         bool flip = false);
	static AudioFile ReadOggFile(const std::string& path);
};
