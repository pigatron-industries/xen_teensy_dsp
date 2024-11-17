#ifndef OscillatorController_h
#define OscillatorController_h

#include "Controller.h"
#include "OscillatorInterface.h"

#include <eurorack_dsp.h>


class OscillatorController : public Controller {
    public:
        OscillatorController() : Controller() {}
        void init(float sampleRate);
        void init();
        void update();
        virtual void process(float **in, float **out, size_t size);


    private:
        // ExpInput<> pitchInput = ExpInput<>(Hardware::hw.POT1);
        ExpInput<> pitchInput = ExpInput<>(Hardware::hw.CVEXP);
        LinearInput<> waveInput = LinearInput<>(Hardware::hw.POT2, -5, 5, 0, 5);
        // LinearInput<> waveInput = LinearInput<>(Hardware::hw.CV2, -5, 5, 0, 5);
        
        WaveSelector<Sine, Triangle, Saw, Pulse> waveSelector;
        WaveOscillator<WaveSelector<Sine, Triangle, Saw, Pulse>&> oscillator = WaveOscillator<WaveSelector<Sine, Triangle, Saw, Pulse>&>(waveSelector);

        OscillatorInterface interface;
};

#endif