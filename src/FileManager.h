#pragma once

#include <GL/glew.h>

#include <string>

class FileManager
{
public:
	static FileManager& Instance();

	// Copy constructor
	FileManager(const FileManager&) = delete;

	// Move constructor
	FileManager(FileManager&&) = delete;

	// Copy assignment
	FileManager& operator =(const FileManager&) = delete;

	// Move assignment
	FileManager& operator =(FileManager&&) = delete;

	std::string ReadAsText(const std::string& path) const;
	unsigned char* ReadImage(const std::string& path,
	                         GLint width,
	                         GLint height,
	                         GLint channels,
	                         bool flip = true) const;
private:
	FileManager() = default;
	~FileManager() = default;
};
