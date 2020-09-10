#pragma once

#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"

#include <GL/glew.h>

#include <memory>
#include <map>
#include <string>

class ResourceManager
{
public:
    static ResourceManager& Instance();
	
    // Copy constructor
    ResourceManager(const ResourceManager&) = delete;
	
    // Move constructor
    ResourceManager(const ResourceManager&&) = delete;
	
    // Copy assignment
    ResourceManager& operator =(const ResourceManager&) = delete;
	
    // Move assignment
    ResourceManager& operator =(const ResourceManager&&) = delete;

	void Initialize();
	void Close();

	std::shared_ptr<ShaderProgram> GetShaderProgram(const std::string& name);
	std::shared_ptr<Texture> GetTexture(const std::string& name);

	std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader);
	std::shared_ptr<Texture> CreateTexture(const std::string& name, const std::string& path, GLuint w, GLuint h, GLint channels = 3, GLuint format = GL_RGB);
	
private:
    ResourceManager() = default;
	~ResourceManager() = default;
	
	std::map<std::string, std::shared_ptr<ShaderProgram>> m_shaderPrograms;
	std::map<std::string, std::shared_ptr<Texture>> m_textures;
};