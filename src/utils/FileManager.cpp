#include "FileManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <sstream>

std::string FileManager::ReadAsText(const std::string& path) const
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

unsigned char* FileManager::ReadImage(const std::string& path, GLint width, GLint height, GLint channels, bool flip) const
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
