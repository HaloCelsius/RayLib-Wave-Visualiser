#include "raylib.h"
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>

int main() {
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

    const int sampleRate = 44100;
    const int bitsPerSample = 16;
    const int numChannels = 1;

    const float durationSeconds = 5.0f;
    const float frequency = 50.0f;
    const short amplitude = 32767;

    int numSamples = (int)(sampleRate * durationSeconds);
    int subchunk2Size = numSamples * numChannels * (bitsPerSample / 8);
    int chunkSize = 36 + subchunk2Size;

    std::vector<std::vector<short>> Samples;
    for (size_t i = 0; i < 4; i++)
    {
        std::vector<short> samples(numSamples);
        Samples.push_back(samples);
    }

    unsigned char header[44] =
    {
        // RIFF chunk
        'R', 'I', 'F', 'F',                                                                 // ChunkID: "RIFF"
        (unsigned char)(chunkSize & 0xFF),                                                  // First BYTE 
        (unsigned char)((chunkSize >> 8) & 0xFF),                                           // Secon BYTE 
        (unsigned char)((chunkSize >> 16) & 0xFF),                                          // Third BYTE 
        (unsigned char)((chunkSize >> 24) & 0xFF),                                          // Fourt BYTE  
        'W', 'A', 'V', 'E',                                                                 // Format: "WAVE"

        // FMT subchunk
        'f', 'm', 't', ' ',                                                                 // Subchunk1ID: "fmt "
        0x10, 0x00, 0x00, 0x00,                                                             // Subchunk1Size: 16 for PCM
        0x01, 0x00,                                                                         // AudioFormat: 1 = PCM
        (unsigned char)(numChannels & 0xFF),                                                // Number Channels: First BYTE
        (unsigned char)((numChannels >> 8) & 0xFF),                                         // Number Channels: Secon BYTE
        (unsigned char)(sampleRate & 0xFF),                                                 // Sample Rate: First BYTE
        (unsigned char)((sampleRate >> 8) & 0xFF),                                          // Sample Rate: Secon BYTE
        (unsigned char)((sampleRate >> 16) & 0xFF),                                         // Sample Rate: Third BYTE
        (unsigned char)((sampleRate >> 24) & 0xFF),                                         // Sample Rate: Fourt BYTE
        (unsigned char)((sampleRate * numChannels * bitsPerSample / 8) & 0xFF),             // Byte Rate: First BYTE
        (unsigned char)(((sampleRate * numChannels * bitsPerSample / 8) >> 8) & 0xFF),      // Byte Rate: Secon BYTE
        (unsigned char)(((sampleRate * numChannels * bitsPerSample / 8) >> 16) & 0xFF),     // Byte Rate: Third BYTE
        (unsigned char)(((sampleRate * numChannels * bitsPerSample / 8) >> 24) & 0xFF),     // Byte Rate: Fourt BYTE
        (unsigned char)((numChannels * bitsPerSample / 8) & 0xFF),                          // Block Align: First BYTE
        (unsigned char)(((numChannels * bitsPerSample / 8) >> 8) & 0xFF),                   // Block Align: Secon BYTE
        (unsigned char)(bitsPerSample & 0xFF),                                              // Bits Per Sample: First BYTE
        (unsigned char)((bitsPerSample >> 8) & 0xFF),                                       // Bits Per Sample: Secon BYTE

        // data subchunk
        'd', 'a', 't', 'a',                                                                 // Subchunk2ID: "data"
        (unsigned char)(subchunk2Size & 0xFF),                                              // Sample Rate: First BYTE
        (unsigned char)((subchunk2Size >> 8) & 0xFF),                                       // Sample Rate: Secon BYTE
        (unsigned char)((subchunk2Size >> 16) & 0xFF),                                      // Sample Rate: Third BYTE,
        (unsigned char)((subchunk2Size >> 24) & 0xFF)                                       // Sample Rate: Fourt BYTE
    };

    std::ofstream sineF("SineWave.wav", std::ios::binary);
    sineF.write((char*)header, sizeof(header));
    for (int i = 0; i < numSamples; i++) {
        float p = (float)i / (float)numSamples*4.f;
        float t = (float)i / sampleRate;
        float angle = t * frequency * (p);
        float cyclePos = angle - floor(angle);
        float value = sin(cyclePos * 2.0f * PI);
       
        /*float value = 0.0f;
        value += sin(cyclePos * 2.0f * PI);
        value += 0.5f * sin((cyclePos * 2.0f) * 2.0f * PI);
        value += 0.25f * sin((cyclePos * 3.0f) * 2.0f * PI);
        value /= 1.75f;*/

        short sample = (short)(value * amplitude);
        Samples[0][i] = sample;
        sineF.write((char*)&sample, sizeof(sample));
    }
    sineF.close();


    std::ofstream squareF("SquareWave.wav", std::ios::binary);
    squareF.write((char*)header, sizeof(header));
    for (int i = 0; i < numSamples; i++) {
        float t = (float)i / sampleRate;
        float angle = t * frequency;
        float cyclePos = angle - floor(angle);
        float value = (cyclePos < 0.5f) ? 1.0f : -1.0f;
        short sample = (short)(value * amplitude);
        Samples[1][i] = sample;
        squareF.write((char*)&sample, sizeof(sample));
    }
    squareF.close();

    std::ofstream sawF("SawtoothWave.wav", std::ios::binary);
    sawF.write((char*)header, sizeof(header));
    for (int i = 0; i < numSamples; i++) {
        float t = (float)i / sampleRate;
        float angle = t * frequency;
        float cyclePos = angle - floor(angle);
        float value = (2.0f * cyclePos) - 1.0f;
        short sample = (short)(value * amplitude);
        Samples[2][i] = sample;
        sawF.write((char*)&sample, sizeof(sample));
    }
    sawF.close();

    std::ofstream triF("TriangleWave.wav", std::ios::binary);
    triF.write((char*)header, sizeof(header));
    for (int i = 0; i < numSamples; i++) {
        float t = (float)i / sampleRate;
        float angle = t * frequency;
        float cyclePos = angle - floor(angle);

        float value;
        if (cyclePos < 0.5f) {
            value = (cyclePos * 4.0f) - 1.0f;
        }
        else {
            value = 3.0f - (cyclePos * 4.0f);
        }

        short sample = (short)(value * amplitude);
        Samples[3][i] = sample;
        triF.write((char*)&sample, sizeof(sample));
    }
    triF.close();

    // Raylib Time: 
    int waveMode = 0;
    InitWindow(800, 600, "Waveform Viewer");
    InitAudioDevice();
    SetMasterVolume(0.1);

    std::vector<Wave> Waves(4);
    for (size_t i = 0; i < 4; i++) {
        Waves[i].frameCount = numSamples;
        Waves[i].sampleRate = sampleRate;
        Waves[i].sampleSize = 16;
        Waves[i].channels = 1;
        Waves[i].data = Samples[i].data();
    }

    Sound sound = LoadSoundFromWave(Waves[waveMode]);
    PlaySound(sound);

    int scrollPos = 0;

    while (!WindowShouldClose()) {
        if (!IsSoundPlaying(sound)) { PlaySound(sound); }

        if (IsKeyPressed(KEY_ONE)) {
            waveMode = 0; 
            UnloadSound(sound); 
            sound = LoadSoundFromWave(Waves[waveMode]); 
            PlaySound(sound);
        }
        if (IsKeyPressed(KEY_TWO)) { 
            waveMode = 1; 
            UnloadSound(sound); 
            sound = LoadSoundFromWave(Waves[waveMode]);
            PlaySound(sound);
        }
        if (IsKeyPressed(KEY_THREE)) {
            waveMode = 2; 
            UnloadSound(sound);
            sound = LoadSoundFromWave(Waves[waveMode]);
            PlaySound(sound); 
        }
        if (IsKeyPressed(KEY_FOUR)) { 
            waveMode = 3;
            UnloadSound(sound); 
            sound = LoadSoundFromWave(Waves[waveMode]); 
            PlaySound(sound); 
        }

        scrollPos = (scrollPos + 2) % numSamples;

        BeginDrawing();
        ClearBackground(BLACK);

        for (size_t i = 0; i < 4; i++)
        {
            for (int x = 0; x < 800; x++) {
                int sampleIndex = (scrollPos + x) % numSamples;
                int y = 300 + (Samples[i][sampleIndex] / 32768.0f) * 200;
                DrawPixel(x, y, i == waveMode ? LIME : GetColor(0xffffff22));
            }
        }

        EndDrawing();
    }

    UnloadSound(sound);
    CloseAudioDevice();
    CloseWindow();
}
