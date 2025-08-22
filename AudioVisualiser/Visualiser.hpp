#pragma once
#include "raylib.h"
#include <vector>

class Visualiser {
public:
	Visualiser(std::vector<std::vector<short>>* samples);
	~Visualiser();

	// Function Declaration
	void Init(int numberSamples, int sampleRate);
	void Run();

	void Update();
	void Draw();
private:

	// Holding audio inside the 'Visualiser.hpp' as its a raylib function
	int waveMode;
	int numSamples;
	int scrollPos;
	Sound audio;

	std::vector<Wave> waves;

	std::vector<std::vector<short>>* samplesRef;
};