#pragma once

#include "stb_truetype.h"
#include <GL/glew.h>

#include <vector>

using Font = struct
{
    const int SIZE = 40;
	const int ATLAS_WIDTH = 2048;
	const int ATLAS_HEIGHT = 2048;
	const int OVERSAMPLE_X = 4;
	const int OVERSAMPLE_Y = 4;
	const char FIRST_CHAR = 0;
	const int CHAR_COUNT = 127;
	std::vector<stbtt_packedchar> charInfo;
	GLuint textureID;
};
