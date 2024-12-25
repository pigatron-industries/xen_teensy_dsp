#ifndef SoundFont_h
#define SoundFont_h

#include "../reader/RiffFileReader.h"
#include <eurorack.h>
#include <eurorack_dsp.h>

class SoundFont {
    public:
        struct Sample {
            char name[20];
            uint32_t startLoop;
            uint32_t endLoop;
            int8_t pitchCorrection;
            uint16_t linkedSample;
            uint16_t sampleType;
            SampleBuffer sampleBuffer;
        };
        struct InstrumentGenerator {
            uint16_t typeId;
            uint16_t value;
        };
        struct InstrumentModulator {

        };
        struct InstrumentZone {
            // this data is derived from the generators and modulators in the sound font file
            int8_t minKey = -1;
            int8_t maxKey = -1;
            int8_t minVelocity = -1;
            int8_t maxVelocity = -1;
            int16_t sampleIndex = -1;
            int16_t pan = 0;
            // generic generators and modulators
            LinkedList<InstrumentGenerator> generators;
            LinkedList<InstrumentModulator> modulators;
        };
        struct Instrument {
            char name[20];
            LinkedList<InstrumentZone> zones;
        };
        struct PresetGenerator {
            uint16_t typeId;
            uint16_t value;
        };
        struct PresetModulator {

        };
        struct PresetZone {
            // this data is derived from the generators and modulators in the sound font file
            int8_t minKey = -1;
            int8_t maxKey = -1;
            int8_t minVelocity = -1;
            int8_t maxVelocity = -1;
            int16_t instrumentIndex = -1;
            // generic generators and modulators
            LinkedList<PresetGenerator> generators;
            LinkedList<PresetModulator> modulators;
        };
        struct Preset { 
            char name[20];
            uint16_t number;
            uint16_t bank;
            LinkedList<PresetZone> zones;
        };

        SoundFont();

        void clear();
        char* getName() { return name; }
        void setName(const char* name);

        int addPreset(const Preset& preset);
        int addPresetZone(int presetIndex);
        void addPresetGenerator(int presetIndex, int zoneIndex, const PresetGenerator& generator);
        void addPresetModulator(int presetIndex, int zoneIndex, const PresetModulator& modulator);
        // void setPresetZoneInstrument(int presetIndex, int zoneIndex, uint16_t instrumentIndex);

        int addInstrument(const Instrument& instrument);
        int addInstrumentZone(int instrumentIndex);
        void addInstrumentGenerator(int instrumentIndex, int zoneIndex, const InstrumentGenerator& generator);
        void addInstrumentModulator(int instrumentIndex, int zoneIndex, const InstrumentModulator& modulator);
        // void setInstrumentZoneSample(int instrumentIndex, int zoneIndex, uint16_t sampleIndex);

        int addSample(const Sample& sample);

        Preset* getPreset(int number);
        Instrument* getInstrument(int index);
        Sample* getSample(int index);

        void print();

    private:
        char name[32];
        LinkedList<Preset> presets;
        LinkedList<Instrument> instruments;
        LinkedList<Sample> samples;
};

#endif