#include "AudioManager.hpp"
#include "Visualiser.hpp"
#include "raylib.h"

int main() {
    // Audio manager takes care of generating the audio and wav files.
    AudioManager* audioManager = new AudioManager();
    audioManager->GenAudio();
    //audioManager->SaveAudio();

    // The Visualiser takes care of all visuals and displaying the audio waves.
    Visualiser* visualiser = new Visualiser(audioManager->GetAllSamples());
    visualiser->Init(audioManager->GetNumSamples(), audioManager->GetSampleRate());

    // Main Loop
    visualiser->Run();

    delete audioManager;
    delete visualiser;
}
