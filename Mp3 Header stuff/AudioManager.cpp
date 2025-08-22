#include "AudioManager.hpp"
#include "raylib.h"
#include <fstream>

AudioManager::AudioManager() {
    numSamples = static_cast<int>(sampleRate * durationSeconds);
}

AudioManager::~AudioManager() {
    // Redundant but explicit.
	for (auto& element : allSamples) {
        element.clear();
	}
    allSamples.clear();
}

void AudioManager::GenAudio()
{
    allSamples = std::vector<std::vector<short>>();

    for (size_t i = 0; i < audioList; i++) {
        std::vector<short> singleSamples(numSamples);
        allSamples.push_back(singleSamples);
    }

    // Sin Wave
    for (int i = 0; i < numSamples; i++) {
        float p = static_cast<float>(i) / static_cast<float>(numSamples) * 4.f;
        float t = static_cast<float>(i) / sampleRate;
        float angle = t * frequency * (p);
        float cyclePos = angle - floor(angle);
        float value = sin(cyclePos * 2.0f * PI);

        // Experiment

        /*float value = 0.0f;
        value += sin(cyclePos * 2.0f * PI);
        value += 0.5f * sin((cyclePos * 2.0f) * 2.0f * PI);
        value += 0.25f * sin((cyclePos * 3.0f) * 2.0f * PI);
        value /= 1.75f;*/

        short sample = static_cast<short>(value * amplitude);
        allSamples[0][i] = sample;
    }

    // Square wave
    for (int i = 0; i < numSamples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        float angle = t * frequency;
        float cyclePos = angle - floor(angle);
        float value = (cyclePos < 0.5f) ? 1.0f : -1.0f;
        short sample = static_cast<short>(value * amplitude);
        allSamples[1][i] = sample;
    }

    // Sawtooth wave
    for (int i = 0; i < numSamples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        float angle = t * frequency;
        float cyclePos = angle - floor(angle);
        float value = (2.0f * cyclePos) - 1.0f;
        short sample = static_cast<short>(value * amplitude);
        allSamples[2][i] = sample;
    }

    // Triangle wave
    for (int i = 0; i < numSamples; i++) {
        float t = static_cast<float>(i) / sampleRate;
        float angle = t * frequency;
        float cyclePos = angle - floor(angle);

        float value;
        if (cyclePos < 0.5f) {
            value = (cyclePos * 4.0f) - 1.0f;
        } else {
            value = 3.0f - (cyclePos * 4.0f);
        }

        short sample = static_cast<short>(value * amplitude);
        allSamples[3][i] = sample;
    }
}

void AudioManager::SaveAudio() {
    // GenAudio needs to have been called first.

    int subchunk2Size = numSamples * numChannels * (bitsPerSample / 8);
    int chunkSize = 36 + subchunk2Size;

    unsigned char header[44] =
    {
        // RIFF chunk
        'R', 'I', 'F', 'F',                                                                             // ChunkID: "RIFF"
        static_cast<unsigned char>(chunkSize & 0xFF),                                                   // First BYTE 
        static_cast<unsigned char>((chunkSize >> 8) & 0xFF),                                            // Secon BYTE 
        static_cast<unsigned char>((chunkSize >> 16) & 0xFF),                                           // Third BYTE 
        static_cast<unsigned char>((chunkSize >> 24) & 0xFF),                                           // Fourt BYTE  
        'W', 'A', 'V', 'E',                                                                             // Format: "WAVE"

        // FMT subchunk
        'f', 'm', 't', ' ',                                                                             // Subchunk1ID: "fmt "
        0x10, 0x00, 0x00, 0x00,                                                                         // Subchunk1Size: 16 for PCM
        0x01, 0x00,                                                                                     // AudioFormat: 1 = PCM
        static_cast<unsigned char>(numChannels & 0xFF),                                                 // Number Channels: First BYTE
        static_cast<unsigned char>((numChannels >> 8) & 0xFF),                                          // Number Channels: Secon BYTE
        static_cast<unsigned char>(sampleRate & 0xFF),                                                  // Sample Rate: First BYTE
        static_cast<unsigned char>((sampleRate >> 8) & 0xFF),                                           // Sample Rate: Secon BYTE
        static_cast<unsigned char>((sampleRate >> 16) & 0xFF),                                          // Sample Rate: Third BYTE
        static_cast<unsigned char>((sampleRate >> 24) & 0xFF),                                          // Sample Rate: Fourt BYTE
        static_cast<unsigned char>((sampleRate * numChannels * bitsPerSample / 8) & 0xFF),              // Byte Rate: First BYTE
        static_cast<unsigned char>(((sampleRate * numChannels * bitsPerSample / 8) >> 8) & 0xFF),       // Byte Rate: Secon BYTE
        static_cast<unsigned char>(((sampleRate * numChannels * bitsPerSample / 8) >> 16) & 0xFF),      // Byte Rate: Third BYTE
        static_cast<unsigned char>(((sampleRate * numChannels * bitsPerSample / 8) >> 24) & 0xFF),      // Byte Rate: Fourt BYTE
        static_cast<unsigned char>((numChannels * bitsPerSample / 8) & 0xFF),                           // Block Align: First BYTE
        static_cast<unsigned char>(((numChannels * bitsPerSample / 8) >> 8) & 0xFF),                    // Block Align: Secon BYTE
        static_cast<unsigned char>(bitsPerSample & 0xFF),                                               // Bits Per Sample: First BYTE
        static_cast<unsigned char>((bitsPerSample >> 8) & 0xFF),                                        // Bits Per Sample: Secon BYTE

        // data subchunk
        'd', 'a', 't', 'a',                                                                             // Subchunk2ID: "data"
        static_cast<unsigned char>(subchunk2Size & 0xFF),                                               // Sample Rate: First BYTE
        static_cast<unsigned char>((subchunk2Size >> 8) & 0xFF),                                        // Sample Rate: Secon BYTE
        static_cast<unsigned char>((subchunk2Size >> 16) & 0xFF),                                       // Sample Rate: Third BYTE
        static_cast<unsigned char>((subchunk2Size >> 24) & 0xFF)                                        // Sample Rate: Fourt BYTE
    };

	const char* files[] = {
        "SineWave.wav",
        "SquareWave.wav",
        "SawtoothWave.wav",
        "TriangleWave.wav"
    };

    for (int i = 0; i < audioList; i++) {
        std::ofstream newSave(files[i], std::ios::binary);
        newSave.write(reinterpret_cast<char*>(header), sizeof(header));

        for (short& j : allSamples[i])
        {
            newSave.write(reinterpret_cast<char*>(&j), sizeof(j));
        }
        newSave.close();
    }
}

// old header file.
/*unsigned char header[44] =
{
    // riff chunk
    'R', 'I', 'F', 'F',              // chunkID: RIFF
    0x24, 0x08, 0x00, 0x00,          // chunkSize: 8 + subchunk1Size + subchunk2Size | placeholder 2048 bytes
    'W', 'A', 'V', 'E',              // format: WAVE

    // fmt chunk
    'f', 'm', 't', ' ',              // subchunk1ID: fmt
    0x10, 0x00, 0x00, 0x00,          // subchunk1Size: 16 for PCM
    0x01, 0x00,                      // audioFormat: 1 = PCM
    0x01, 0x00,                      // numChannels: 1 = Mono
    0x44, 0xAC, 0x00, 0x00,          // sampleRate: 44100 Hz
    0x88, 0x58, 0x01, 0x00,          // byteRate: sampleRate * numChannels * bitsPerSample/8
    0x02, 0x00,                      // blockAlign: numChannels * bitsPerSample/8
    0x10, 0x00,                      // bitsPerSample: 16

    // data chunk
    'd', 'a', 't', 'a',              // subchunk2ID: data
    0x00, 0x08, 0x00, 0x00           // subchunk2Size: numSamples * numChannels * bitsPerSample/8 | placeholder 2048 bytes
};*/