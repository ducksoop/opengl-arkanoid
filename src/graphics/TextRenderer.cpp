#include "TextRenderer.h"
#include "FileManager.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <iostream>

void TextRenderer::Initialize(const std::string& path, ShaderProgram* shaderProgram)
{
	m_shaderProgram = shaderProgram;
	InitializeFont(path);
	InitializeGlyphs();
	InitializeVAO();
}

void TextRenderer::Render(const std::string& text, const glm::vec2& position, const glm::vec3& color, float scale)
{
	m_shaderProgram->Use();
	m_shaderProgram->SetUniform("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	auto x = position.x;
	auto i = 0;

	for (auto c : text) {
		auto glyph = m_glyphs[c];

		// Update VBO for each character
		auto width = glyph.size.x * scale;
		auto height = glyph.size.y * scale;

		auto xPosition = x;
		auto yPosition = position.y;

		GLfloat vertices[] = {
			xPosition,         yPosition - height, glyph.uvs[0].x, glyph.uvs[1].y,
			xPosition,         yPosition,          glyph.uvs[0].x, glyph.uvs[0].y,
			xPosition + width, yPosition - height, glyph.uvs[1].x, glyph.uvs[1].y,
			xPosition + width, yPosition,		   glyph.uvs[1].x, glyph.uvs[0].y
		};

		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) * i, sizeof(vertices), vertices);

		x += width;
		i++;
	}

	// Render quads
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 * text.length());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_shaderProgram->End();
}

void TextRenderer::InitializeFont(const std::string& path)
{
	auto fontData = FileManager::ReadAsBinary(path);
	auto atlasData = std::vector<unsigned char>(m_font.ATLAS_WIDTH * m_font.ATLAS_HEIGHT);
	m_font.charInfo = std::vector<stbtt_packedchar>(m_font.CHAR_COUNT);

	stbtt_pack_context context;
	if (!stbtt_PackBegin(&context, &atlasData.front(), m_font.ATLAS_WIDTH, m_font.ATLAS_HEIGHT, 0, 1, nullptr)) {
		std::cerr << "Failed to initialize font" << std::endl;
	}

	stbtt_PackSetOversampling(&context, m_font.OVERSAMPLE_X, m_font.OVERSAMPLE_Y);
	if (!stbtt_PackFontRange(&context, &fontData.front(), 0,
		m_font.SIZE, m_font.FIRST_CHAR, m_font.CHAR_COUNT, &m_font.charInfo.front())) {
		std::cerr << "Failed to pack font" << std::endl;
	}

	stbtt_PackEnd(&context);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_font.ATLAS_WIDTH, m_font.ATLAS_HEIGHT, 0,
		GL_RED, GL_UNSIGNED_BYTE, &atlasData.front());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextRenderer::InitializeGlyphs()
{
	float offsetX = 0;
	float offsetY = 0;

	for (auto c = m_font.FIRST_CHAR; c < m_font.CHAR_COUNT; c++)
	{
		auto glyphInfo = GetGlyphInfo(c, offsetX, offsetY);
		offsetX = glyphInfo.offsetX;
		offsetY = glyphInfo.offsetY;

		m_glyphs[c] = glyphInfo;
	}

	// Add more width to space it
	auto glyph = m_glyphs['A'];
	m_glyphs[' '].size.x += glyph.size.x;
}

void TextRenderer::InitializeVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * m_font.CHAR_COUNT, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GlyphInfo TextRenderer::GetGlyphInfo(char character, float offsetX, float offsetY)
{
	stbtt_aligned_quad quad;

	stbtt_GetPackedQuad(&m_font.charInfo.front(), m_font.ATLAS_WIDTH, m_font.ATLAS_HEIGHT,
		character - m_font.FIRST_CHAR, &offsetX, &offsetY, &quad, 1);

	auto info = GlyphInfo();
	info.offsetX = offsetX;
	info.offsetY = offsetY;
	info.size = glm::vec2(quad.x1 - quad.x0, quad.y1 - quad.y0);
	info.uvs[0] = { quad.s0, quad.t1 };
	info.uvs[1] = { quad.s1, quad.t0 };

	return info;
}

TextRenderer::~TextRenderer()
{
	glDeleteTextures(1, &m_textureID);
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}
