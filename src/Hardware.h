#ifndef Hardware_h
#define Hardware_h

#include <Arduino.h>
#include <Audio.h>
#include <eurorack.h>
#include <eurorack_sd.h>

#include "hwconfig.h"
#include "interface/TFTDisplay.h"
#include "filesystem/soundfont/SoundFontManager.h"

#define MEMPOOL_SAMPLES_SIZE 8*1024*1024

class Hardware {
    public:
        static Hardware hw;
        void init();

        AnalogInput(CVEXP, A10)
        AnalogInput(TRIG, A11)

        AnalogInput(CV1, A16)
        AnalogInput(CV2, A17)
        AnalogInput(CV3, A12)
        AnalogInput(CV4, A13)

        AnalogInput(POT1, A14)
        AnalogInput(POT2, A15)
        AnalogInput(POT3, A2)
        AnalogInput(POT4, A3)


        // SD Card
        FileSystem fsSamples = FileSystem(SD_CS_PIN, "/samples");

        // Memory pool
        static uint8_t memPoolSamplesBuffer[MEMPOOL_SAMPLES_SIZE];
        MemPool<> memPoolSamples = MemPool<>(Hardware::memPoolSamplesBuffer, MEMPOOL_SAMPLES_SIZE);

        // File management
        SoundFontManager soundFontManager = SoundFontManager(fsSamples, memPoolSamples);

        // Direct connections
        RotaryEncoderButton encoder1 = RotaryEncoderButton(ENCODER1_PIN1, ENCODER1_PIN2, ENCODER1_BTN_PIN);
        RotaryEncoderButton encoder2 = RotaryEncoderButton(ENCODER2_PIN1, ENCODER2_PIN2, ENCODER2_BTN_PIN);

        TFTDisplay display;

        // Audio
        AudioControlSGTL5000 sgtl5000_1;
        AudioOutputI2S       i2s1;

        
};

#endif