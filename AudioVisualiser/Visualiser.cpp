#include "Visualiser.hpp"

Visualiser::Visualiser(std::vector<std::vector<short>>* samples) {
    samplesRef = samples;
    waves = std::vector<Wave>(4);
    waveMode = 0;
    scrollPos = 0;
}

Visualiser::~Visualiser() {
    UnloadSound(audio);
    CloseAudioDevice();

    delete samplesRef;
}

void Visualiser::Init(int numberSamples, int sampleRate)
{
    numSamples = numberSamples;
    waveMode = 0;
    InitWindow(800, 600, "Waveform Viewer");
    InitAudioDevice();

    SetMasterVolume(0.1f);

    std::vector<Wave> Waves(4);
    for (size_t i = 0; i < 4; i++) {
        Waves[i].frameCount = numSamples;
        Waves[i].sampleRate = sampleRate;
        Waves[i].sampleSize = 16;
        Waves[i].channels = 1;
        Waves[i].data = samplesRef->at(i).data();
    }

    Sound sound = LoadSoundFromWave(Waves[waveMode]);
    PlaySound(sound);
}

void Visualiser::Run(){
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    CloseWindow();
}

void Visualiser::Update() {
    if (!IsSoundPlaying(audio)) { PlaySound(audio); }

    if (IsKeyPressed(KEY_ONE)) {
        waveMode = 0;
        UnloadSound(audio);
        audio = LoadSoundFromWave(waves[waveMode]);
        PlaySound(audio);
    }
    if (IsKeyPressed(KEY_TWO)) {
        waveMode = 1;
        UnloadSound(audio);
        audio = LoadSoundFromWave(waves[waveMode]);
        PlaySound(audio);
    }
    if (IsKeyPressed(KEY_THREE)) {
        waveMode = 2;
        UnloadSound(audio);
        audio = LoadSoundFromWave(waves[waveMode]);
        PlaySound(audio);
    }
    if (IsKeyPressed(KEY_FOUR)) {
        waveMode = 3;
        UnloadSound(audio);
        audio = LoadSoundFromWave(waves[waveMode]);
        PlaySound(audio);
    }

    scrollPos = (scrollPos + 2) % numSamples;
}

void Visualiser::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    for (size_t i = 0; i < 4; i++) {
        for (int x = 0; x < 800; x++) {
            int sampleIndex = (scrollPos + x) % numSamples;
            int y = 300 + ( samplesRef->at(i)[sampleIndex] / 32768.0f) * 200;
            DrawPixel(x, y, i == waveMode ? LIME : GetColor(0xffffff22));
        }
    }
    EndDrawing();
}
