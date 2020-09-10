#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "FileManager.h"

#include <memory>
#include <string>
#include <iostream>

ResourceManager& ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::Initialize()
{
}

void ResourceManager::Close()
{
	for (const auto& shader : m_shaderPrograms)
		shader.second->Destroy();

	for (const auto& texture : m_textures)
		texture.second->Destroy();
}

std::shared_ptr<ShaderProgram> ResourceManager::GetShaderProgram(const std::string& name)
{
	return m_shaderPrograms[name];
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
{
	return m_textures[name];
}

std::shared_ptr<ShaderProgram> ResourceManager::CreateShaderProgram(const std::string& name, const Shader& vertexShader,
	const Shader& fragmentShader)
{
	std::shared_ptr<ShaderProgram> shader(new ShaderProgram(vertexShader, fragmentShader));
	m_shaderPrograms[name] = shader;

	return shader;
}

std::shared_ptr<Texture> ResourceManager::CreateTexture(const std::string& name, const std::string& path, GLuint w, GLuint h,
	GLint channels, GLuint format)
{
	unsigned char* image = FileManager::Instance().ReadImage(path, w, h, channels);

	std::shared_ptr<Texture> texture(new Texture(w, h, image, format));
	m_textures[name] = texture;

	return texture;
}
