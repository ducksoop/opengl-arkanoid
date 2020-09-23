#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <map>

#include "ShaderProgram.h"
#include "Font.h"
#include "GlyphInfo.h"
#include "Singleton.h"

class TextRenderer : public Singleton<TextRenderer>
{
public:
	void Initialize(const std::string& path, ShaderProgram* shaderProgram);
	void Render(const std::string& text,
				const glm::vec2& position,
				const glm::vec3& color,
				float scale = 1.0f);
private:
	void InitializeFont(const std::string& path);
	void InitializeGlyphs();
	void InitializeVAO();
	
	GlyphInfo GetGlyphInfo(char character, float offsetX, float offsetY);

	TextRenderer() = default;
	~TextRenderer();

	ShaderProgram* m_shaderProgram;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_textureID;

	Font m_font;

	std::map<char, GlyphInfo> m_glyphs;

	friend Singleton;
};
