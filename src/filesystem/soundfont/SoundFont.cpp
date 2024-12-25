#include "SoundFont.h"
#include "SoundFontConstants.h"

SoundFont::SoundFont() {
}

void SoundFont::clear() {
    presets.clear();
    instruments.clear();
}

void SoundFont::setName(const char* name) {
    strncpy(this->name, name, sizeof(this->name) - 1);
}

int SoundFont::addPreset(const SoundFont::Preset& preset) {
    int index = presets.size();
    presets.add(preset);
    return index;
}

int SoundFont::addPresetZone(const int presetIndex) {
    PresetZone zone;
    int index = presets[presetIndex].zones.size();
    presets[presetIndex].zones.add(zone);
    return index;
}

void SoundFont::addPresetGenerator(int presetIndex, int zoneIndex, const PresetGenerator& generator) {
    PresetZone& presetZone = presets[presetIndex].zones[zoneIndex];
    switch (generator.typeId)
    {
        case GEN_VELOCITY_RANGE:
            presetZone.minVelocity = generator.value & 0x7F;
            presetZone.maxVelocity = (generator.value >> 8) & 0x7F;
            break;
        case GEN_KEY_RANGE:
            presetZone.minKey = generator.value & 0x7F;
            presetZone.maxKey = (generator.value >> 8) & 0x7F;
            break;
        case GEN_INSTRUMENT_ID:
            presetZone.instrumentIndex = generator.value;
            break;
        default:
            presets[presetIndex].zones[zoneIndex].generators.add(generator);
            break;
    }
}

void SoundFont::addPresetModulator(int presetIndex, int zoneIndex, const PresetModulator& modulator) {
    presets[presetIndex].zones[zoneIndex].modulators.add(modulator);
}

int SoundFont::addInstrument(const Instrument& instrument) {
    int index = instruments.size();
    instruments.add(instrument);
    return index;
}

int SoundFont::addInstrumentZone(int instrumentIndex) {
    InstrumentZone zone;
    int index = instruments[instrumentIndex].zones.size();
    instruments[instrumentIndex].zones.add(zone);
    return index;
}

void SoundFont::addInstrumentGenerator(int instrumentIndex, int zoneIndex, const InstrumentGenerator& generator) {
    InstrumentZone& instrumentZone = instruments[instrumentIndex].zones[zoneIndex];
    switch (generator.typeId)
    {
        case GEN_PAN:
            instrumentZone.pan = generator.value;
            break;
        case GEN_VELOCITY_RANGE:
            instrumentZone.minVelocity = generator.value & 0x7F;
            instrumentZone.maxVelocity = (generator.value >> 8) & 0x7F;
            break;
        case GEN_KEY_RANGE:
            instrumentZone.minKey = generator.value & 0x7F;
            instrumentZone.maxKey = (generator.value >> 8) & 0x7F;
            break;
        case GEN_SAMPLE_ID:
            instrumentZone.sampleIndex = generator.value;
            break;
        default:
            instrumentZone.generators.add(generator);
            break;
    }
}

void SoundFont::addInstrumentModulator(int instrumentIndex, int zoneIndex, const InstrumentModulator& modulator) {
    instruments[instrumentIndex].zones[zoneIndex].modulators.add(modulator);
}

int SoundFont::addSample(const Sample& sample) {
    int index = samples.size();
    samples.add(sample);
    return index;
}

SoundFont::Preset* SoundFont::getPreset(int number) {
    for(int i = 0; i < presets.size(); i++) {
        if(presets[i].number == number) {
            return &presets[i];
        }
    }
    return nullptr;
}

SoundFont::Instrument* SoundFont::getInstrument(int index) {
    return &instruments[index];
}

SoundFont::Sample* SoundFont::getSample(int index) {
    return &samples[index];
}


void SoundFont::print() {
    for(int i = 0; i < presets.size(); i++) {
        SoundFont::Preset& preset = presets[i];
        Serial.print("Preset: number=");
        Serial.print(preset.number);
        Serial.print(", bank=");
        Serial.print(preset.bank);
        Serial.print(", name=");
        Serial.println(preset.name);
        for(int j = 0; j < preset.zones.size(); j++) {
            SoundFont::PresetZone& zone = preset.zones[j];
            Serial.print("   Zone: instrument=");
            Serial.print(zone.instrumentIndex);
            Serial.print(", minKey=");
            Serial.print(zone.minKey);
            Serial.print(", maxKey=");
            Serial.print(zone.maxKey);
            Serial.print(", minVelocity=");
            Serial.print(zone.minVelocity);
            Serial.print(", maxVelocity=");
            Serial.print(zone.maxVelocity);
            for(int k = 0; k < zone.generators.size(); k++) {
                SoundFont::PresetGenerator& generator = zone.generators[k];
                Serial.print(" Gen(");
                Serial.print(generator.typeId);
                Serial.print(", ");
                Serial.print(generator.value, 16);
                Serial.print(")");
            }
            // for(int k = 0; k < zone.modulators.size(); k++) {
            //     SoundFont::PresetModulator& modulator = zone.modulators[k];
            //     Serial.print("      Modulator: type=");
            //     Serial.print(modulator.typeId);
            //     Serial.print(", value=");
            //     Serial.print((int16_t)generator.value);
            //     Serial.println();
            // }
            Serial.println();
        }
    }

    for(int i = 0; i < instruments.size(); i++) {
        SoundFont::Instrument& instrument = instruments[i];
        Serial.print("Instrument: idx=");
        Serial.print(i);
        Serial.print(", name=");
        Serial.println(instrument.name);
        for(int j = 0; j < instrument.zones.size(); j++) {
            SoundFont::InstrumentZone& zone = instrument.zones[j];
            Serial.print("   Zone: idx=");
            Serial.print(j);
            Serial.print(", minKey=");
            Serial.print(zone.minKey);
            Serial.print(", maxKey=");
            Serial.print(zone.maxKey);
            Serial.print(", minVelocity=");
            Serial.print(zone.minVelocity);
            Serial.print(", maxVelocity=");
            Serial.print(zone.maxVelocity);
            Serial.print(", sampleIndex=");
            Serial.print(zone.sampleIndex);
            Serial.print(", pan=");
            Serial.print(zone.pan);
            for(int k = 0; k < zone.generators.size(); k++) {
                SoundFont::InstrumentGenerator& generator = zone.generators[k];
                Serial.print(" Gen(");
                Serial.print(generator.typeId);
                Serial.print(", ");
                Serial.print(generator.value, 16);
                Serial.print(")");
            }
            // for(int k = 0; k < zone.modulators.size(); k++) {
            //     SoundFont::InstrumentModulator& modulator = zone.modulators[k];
            //     Serial.print("      Mod: type=");
            //     Serial.print(modulator.typeId);
            //     Serial.print(", value=");
            //     Serial.print((int16_t)generator.value);
            //     Serial.println();
            // }
            Serial.println();
        }
    }

    for(int i = 0; i < samples.size(); i++) {
        SoundFont::Sample& sample = samples[i];
        // Serial.print("Sample: name=");
        // Serial.print(sample.name);
        // Serial.println();
    }
}