#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "FileManager.h"

#include <memory>
#include <string>
#include <iostream>

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

ShaderProgram* ResourceManager::GetShaderProgram(const std::string& name)
{
	return m_shaderPrograms[name].get();
}

Texture* ResourceManager::GetTexture(const std::string& name)
{
	return m_textures[name].get();
}

ShaderProgram* ResourceManager::CreateShaderProgram(const std::string& name, const Shader& vertexShader,
	const Shader& fragmentShader)
{
	m_shaderPrograms[name] = std::unique_ptr<ShaderProgram>(
		new ShaderProgram(vertexShader, fragmentShader)
	);

	return m_shaderPrograms[name].get();
}

Texture* ResourceManager::CreateTexture(const std::string& name, const std::string& path, GLuint w, GLuint h,
	GLint channels, GLuint format)
{
	auto image = FileManager::ReadImage(path, w, h, channels);

	m_textures[name] = std::unique_ptr<Texture>(
		new Texture(w, h, image, format)
	);

	return m_textures[name].get();
}
