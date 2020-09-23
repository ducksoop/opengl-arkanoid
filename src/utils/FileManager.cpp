#include "FileManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_vorbis.c>

#include <iostream>
#include <fstream>
#include <sstream>

std::string FileManager::ReadAsText(const std::string& path)
{
	std::string content;
	std::ifstream file(path);

	if (file.is_open())
	{
		std::stringstream stream;

		// Read file's buffer contents into stream
		stream << file.rdbuf();
		file.close();

		// Convert stream into string
		content = stream.str();
	}
	else
	{
		std::cerr << "Unable to open text file: " << path << std::endl;
	}
	
	return content;
}

std::vector<unsigned char> FileManager::ReadAsBinary(const std::string& path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		std::cerr << "Unable to open font file: " << path << std::endl;
	}

	auto size = file.tellg();
	auto bytes = std::vector<unsigned char>(size);
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&bytes.front()), size);
	file.close();

	return bytes;
}

unsigned char* FileManager::ReadImage(const std::string& path, GLint width, GLint height, GLint channels, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (image == nullptr)
	{
		std::cerr << "Unable to open image file: " << path << std::endl
			<< "Reason: " << stbi_failure_reason() << std::endl;
	}

	return image;
}

AudioFile FileManager::ReadOggFile(const std::string& path)
{
	AudioFile audioFile;
	short* output;
	audioFile.samples = stb_vorbis_decode_filename(path.c_str(),
	                                               &audioFile.channels,
	                                               &audioFile.sampleRate,
	                                               &output);
	audioFile.data = std::unique_ptr<short>(output);

	return audioFile;
}
