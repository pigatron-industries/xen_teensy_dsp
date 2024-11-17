#include "MainController.h"
#include "hwconfig.h"
#include "Hardware.h"

#include <Arduino.h>
#include <SPI.h>
#include <math.h>

#define TRANSMIT_TIME 10000

MainController* MainController::mainController = nullptr;

MainController::MainController(float sampleRate) : DoubleEncoderController(Hardware::hw.encoder1, Hardware::hw.encoder2), 
// AudioStream(0, NULL) {
AudioSynthWaveformSine() {
    MainController::mainController = this;
    this->sampleRate = sampleRate;
    initOnModeSelect = false;
}

void MainController::init() {
    Hardware::hw.init();
    DoubleEncoderController::init();
    this->frequency(440);
    this->amplitude(0.5);
}

void MainController::controllerInit() {
    saveState();

    if(controllers.getSelected()->isInited() > 0) {
        controllers.getSelected()->init();
    } else {
        controllers.getSelected()->init(sampleRate);
    }

    int intervalMicros = 1000000/sampleRate;
}

void MainController::loop() {
    doEncoder1Event(Hardware::hw.encoder1.getEncoderEvent());
    doEncoder2Event(Hardware::hw.encoder2.getEncoderEvent());

    controllers.getSelected()->update();

    PROFILE_PRINT
}

void MainController::update() {
    PROFILE_START

    // TODO fill block in with input audio
    controllers.getSelected()->process(block_in, block_out, AUDIO_BLOCK_SAMPLES);

    audio_block_t* block_left;
    audio_block_t* block_right;
    block_left = allocate();
    block_right = allocate();

    if(block_left && block_right) {
        for (int i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
            float value_left = std::fmax(-1.0f, std::fmin(1.0f, block_out[0][i]));
            float value_right = std::fmax(-1.0f, std::fmin(1.0f, block_out[1][i]));
            block_left->data[i] = static_cast<int16_t>(value_left * 32767);
            block_right->data[i] = static_cast<int16_t>(value_right * 32767);
        }
        transmit(block_left, 0);
        transmit(block_right, 1);
        release(block_left);
        release(block_right);
    }

    PROFILE_END
}
