#ifndef SamplePlayerController_h
#define SamplePlayerController_h

#include "Controller.h"
#include "SamplePlayerInterface.h"
#include "filesystem/soundfont/SoundFontPlayer.h"

#include <Audio.h>
#include <eurorack_dsp.h>

#define PARAMETERS 3


class SamplePlayerController : public ParameterizedController<PARAMETERS> {
    public:
        enum Parameter {
            SAMPLEFILE,
            PRESET,
            PITCH_MODE
        };

        enum PitchMode {
            MONO_FIXED,
            MONO_BEND,
            POLY
        };

        SamplePlayerController() : ParameterizedController() {}
        void init(float sampleRate);
        void init();

        int cycleParameter(int amount);
        virtual void cycleValue(int amount);
        void selectValue();

        void update();
        virtual void process(float **in, float **out, size_t size);


    private:

        ExpInput<> pitchInput = ExpInput<>(Hardware::hw.CVEXP);
        LinearInput<> velocityInput = LinearInput<>(Hardware::hw.CV1, -5, 5, 0, 1);
        AnalogGateInput<> triggerInput = AnalogGateInput<>(Hardware::hw.TRIG);

        SamplePlayerInterface interface;

        SoundFont* soundFont = nullptr;

        SoundFontPlayer player;
        

        void loadSampleFile(int index);
};

#endif