#ifndef MainController_h
#define MainController_h

#include <eurorack.h>
#include <inttypes.h>
#include <AudioStream.h>

#include "Controller.h"
#include "Hardware.h"
#include "apps.h"

class MainController : DoubleEncoderController<Controller, CONTROLLERS>, public AudioSynthWaveformSine {

public:
    MainController(float sampleRate);
    void init();
    void loop();
    virtual void update();


private:
    float sampleRate;

    void controllerInit();

    static MainController* mainController;


    float block_in_left[AUDIO_BLOCK_SAMPLES];
    float block_in_right[AUDIO_BLOCK_SAMPLES];
    float block_out_left[AUDIO_BLOCK_SAMPLES];
    float block_out_right[AUDIO_BLOCK_SAMPLES];
    float* block_in[2] = {block_in_left, block_in_right};
    float* block_out[2] = {block_out_left, block_out_right};


    AudioConnection patchCord1 = AudioConnection(*this, 0, Hardware::hw.i2s1, 0);
    AudioConnection patchCord2 = AudioConnection(*this, 1, Hardware::hw.i2s1, 1);
};

#endif
