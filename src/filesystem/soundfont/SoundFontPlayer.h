#ifndef SoundFontPlayer_h
#define SoundFontPlayer_h

#include "SoundFont.h"

class SoundFontPlayer {
    public:
        SoundFontPlayer() {}
        void init(float sampleRate);

        void setSoundFont(SoundFont* soundFont, int preset);

        void trigger(float velocity, float frequency);
        void process();
        float* getOutput() { return output; }

    private:
        SoundFont* soundFont = nullptr;
        SoundFont::Preset* preset = nullptr;

        Array<SamplePlayer, 2> player;
        boolean stereo = false;

        float output[2];

        boolean zoneMatches(SoundFont::PresetZone& zone, float velocity, float frequency);
        boolean zoneMatches(SoundFont::InstrumentZone& zone, float velocity, float frequency);
};

#endif

