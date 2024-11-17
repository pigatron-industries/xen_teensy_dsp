#include "SamplePlayerController.h"


void SamplePlayerController::init(float sampleRate) {
    this->sampleRate = sampleRate;

    // waveform1.begin(WAVEFORM_SINE);
    // waveform1.frequency(880);
    // waveform1.amplitude(0.9);

    interface.init();
    init();
}

void SamplePlayerController::init() {
    Serial.println("SamplePlayerController::init");
    interface.render();
}

void SamplePlayerController::update() {
    //Serial.println("SamplePlayerController::update");   
    // Update the controller
}

void SamplePlayerController::process(float **in, float **out, size_t size) {
    // Process the controller
}
