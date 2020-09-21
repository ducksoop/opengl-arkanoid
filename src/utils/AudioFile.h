#pragma once

#include <memory>

struct AudioFile
{
	int channels;
	int sampleRate;
	int samples;
	std::unique_ptr<short> data;

	int GetSampleCount() const
	{
		return channels * samples;
	}
};
