#pragma once
#include <vector>

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	// Function Declaration
	void GenAudio();
	void SaveAudio();

	int GetNumSamples() const { return numSamples; }
	int GetSampleRate() const { return sampleRate; }
	std::vector<std::vector<short>>* GetAllSamples() { return &allSamples; }
private:
	const int audioList = 4;

	const int sampleRate = 44100;
	const int bitsPerSample = 16;
	const int numChannels = 1;

	const float durationSeconds = 5.0f;
	const float frequency = 50.0f;
	const short amplitude = 32767;

	int numSamples = 0;
	std::vector<std::vector<short>> allSamples;
};
