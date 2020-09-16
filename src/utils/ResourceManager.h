#pragma once

#include "Singleton.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"

#include <GL/glew.h>

#include <memory>
#include <map>
#include <string>

class ResourceManager : public Singleton<ResourceManager>
{
public:
	void Initialize();
	void Close();

	ShaderProgram* GetShaderProgram(const std::string& name);
	Texture* GetTexture(const std::string& name);

	ShaderProgram* CreateShaderProgram(const std::string& name, const Shader& vertexShader, const Shader& fragmentShader);
	Texture* CreateTexture(const std::string& name, const std::string& path, GLuint w, GLuint h, GLint channels = 3, GLuint format = GL_RGB);
	
private:
    ResourceManager() = default;
	~ResourceManager() = default;
	
	std::map<std::string, std::unique_ptr<ShaderProgram>> m_shaderPrograms;
	std::map<std::string, std::unique_ptr<Texture>> m_textures;

	friend Singleton;
};