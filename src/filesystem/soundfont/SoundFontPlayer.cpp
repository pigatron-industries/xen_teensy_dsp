#include "SoundFontPlayer.h"

#define LEFT 0
#define RIGHT 1

void SoundFontPlayer::init(float sampleRate) {
    player[LEFT].init(sampleRate);
    player[RIGHT].init(sampleRate);
}

void SoundFontPlayer::setSoundFont(SoundFont* soundFont, int preset) {
    player[LEFT].stop();
    player[RIGHT].stop();
    this->soundFont = soundFont;
    this->preset = soundFont->getPreset(preset);
}


void SoundFontPlayer::trigger(float velocity, float frequency) {
    Serial.println("SoundFontPlayer::trigger");
    Serial.println(velocity);
    Serial.println(frequency);

    if (soundFont == nullptr || preset == nullptr) {
        return;
    }

    for (int i = 0; i < preset->zones.size(); i++) {
        SoundFont::PresetZone& zone = preset->zones[i];
        if (zoneMatches(zone, velocity, frequency)) {
            Serial.println("zone.instrumentIndex");
            Serial.println(zone.instrumentIndex);
            SoundFont::Instrument* instrument = soundFont->getInstrument(zone.instrumentIndex);
            if (instrument != nullptr) {
                for (int j = 0; j < instrument->zones.size(); j++) {
                    SoundFont::InstrumentZone& instrumentZone = instrument->zones[j];
                    if (zoneMatches(instrumentZone, velocity, frequency)) {
                        SoundFont::Sample* sample = soundFont->getSample(instrumentZone.sampleIndex);
                        if (sample != nullptr) {
                            player[LEFT].stop();
                            player[RIGHT].stop();
                            player[LEFT].setSample(&sample->sampleBuffer);
                            player[LEFT].setFrequency(frequency);
                            player[LEFT].play();
                            if(sample->linkedSample != -1) {
                                sample = soundFont->getSample(sample->linkedSample);
                                player[RIGHT].setSample(&sample->sampleBuffer);
                                player[RIGHT].setFrequency(frequency);
                                player[RIGHT].play();
                                stereo = true;
                            } else {
                                stereo = false;
                            }
                        }
                    }
                }
            }
        }
    }
}


boolean SoundFontPlayer::zoneMatches(SoundFont::PresetZone& zone, float velocity, float frequency) {
    int velocityInt = velocity * 127.0;
    int key = frequencyToMidi(frequency);
    return (zone.minVelocity == -1 && zone.maxVelocity == -1 || zone.minVelocity <= velocityInt && zone.maxVelocity >= velocityInt) &&
           (zone.minKey == -1 && zone.maxKey == -1 || zone.minKey <= key && zone.maxKey >= key);
}


boolean SoundFontPlayer::zoneMatches(SoundFont::InstrumentZone& zone, float velocity, float frequency) {
    if(zone.sampleIndex == -1) {
        return false;
    }
    int velocityInt = velocity * 127.0;
    int key = frequencyToMidi(frequency);
    return (zone.minVelocity == -1 && zone.maxVelocity == -1 || zone.minVelocity <= velocityInt && zone.maxVelocity >= velocityInt) &&
           (zone.minKey == -1 && zone.maxKey == -1 || zone.minKey <= key && zone.maxKey >= key);
}


void SoundFontPlayer::process() {
    output[LEFT] = player[LEFT].process();
    if(stereo) {
        output[RIGHT] = player[RIGHT].process();
    } else {
        output[RIGHT] = output[LEFT];
    }
}