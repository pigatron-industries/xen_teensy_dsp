#include "SoundFont2FileReader.h"
#include "util/stringutil.h"
#include "util/pitchutil.h"
#include "SoundFontConstants.h"

#define PHDR_RECORD_LENGTH 38
#define PBAG_RECORD_LENGTH 4
#define PGEN_RECORD_LENGTH 4
#define PMOD_RECORD_LENGTH 10

#define INST_RECORD_LENGTH 22
#define IBAG_RECORD_LENGTH 4
#define IGEN_RECORD_LENGTH 4
#define IMOD_RECORD_LENGTH 10

#define SHDR_RECORD_LENGTH 46

SoundFont2FileReader::SoundFont2FileReader(SoundFont& soundFont, MemPool<>& memPool) : 
    RiffFileReader("sfbk"),
    memPool(memPool),
    soundFont(soundFont) {
}

void SoundFont2FileReader::preProcess(FsFile& file) {
    soundFont.clear();
    char name[32];
    file.getName(name, 32);
    soundFont.setName(name);
}

bool SoundFont2FileReader::readChunkData(FsFile& file, ChunkHeader chunk) {
    if(RiffFileReader::readChunkData(file, chunk)) {
       return true; 
    } else if(beginsWith(chunk.id, "smpl")) {
        smplChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "sm24")) {
        sm24Chunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "phdr")) {
        phdrChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "pbag")) {
        pbagChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "pmod")) {
        pmodChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "pgen")) {
        pgenChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "inst")) {
        instChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "ibag")) {
        ibagChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "imod")) {
        imodChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "igen")) {
        igenChunk = chunk;
        return true;
    } else if(beginsWith(chunk.id, "shdr")) {
        shdrChunk = chunk;
        return true;
    }
}

void SoundFont2FileReader::postProcess(FsFile& file) {
    processPresets(file);
    processInstruments(file);
    processSamples(file);
}

void SoundFont2FileReader::processPresets(FsFile& file) {
    // Serial.println("processPresets");
    int presetRecords = (phdrChunk.dataSize / PHDR_RECORD_LENGTH) - 1;
    for(int i = 0; i < presetRecords; i++) {
        file.seek(phdrChunk.dataPosition + (PHDR_RECORD_LENGTH*i));

        SoundFont::Preset preset;
        file.readBytes((byte*)&preset.name, sizeof(preset.name));
        file.readBytes((byte*)&preset.number, sizeof(preset.number));
        file.readBytes((byte*)&preset.bank, sizeof(preset.bank));
        int presetIndex = soundFont.addPreset(preset);
        // Serial.print("   ");
        // Serial.println(preset.name);

        uint16_t zoneStartIndex;
        file.readBytes((byte*)&zoneStartIndex, sizeof(zoneStartIndex));

        uint16_t zoneEndIndex;
        file.seek(phdrChunk.dataPosition + (PHDR_RECORD_LENGTH*(i+1)) + 24); //position of zoneIndex in next record
        file.readBytes((byte*)&zoneEndIndex, sizeof(zoneEndIndex));

        processPresetZones(file, presetIndex, zoneStartIndex, zoneEndIndex);
    }
}

void SoundFont2FileReader::processPresetZones(FsFile& file, int presetIndex, uint16_t zoneStartIndex, uint16_t zoneEndIndex) {
    //Serial.println("processPresetZones");
    for(int i = zoneStartIndex; i < zoneEndIndex; i++) {
        int presetZoneIndex = soundFont.addPresetZone(presetIndex);

        uint16_t generatorStartIndex;
        uint16_t modulatorStartIndex;
        file.seek(pbagChunk.dataPosition + (PBAG_RECORD_LENGTH*i));
        file.readBytes((byte*)&generatorStartIndex, sizeof(generatorStartIndex));
        file.readBytes((byte*)&modulatorStartIndex, sizeof(modulatorStartIndex));

        uint16_t generatorEndIndex;
        uint16_t modulatorEndIndex;
        file.seek(pbagChunk.dataPosition + (PBAG_RECORD_LENGTH*(i+1)));
        file.readBytes((byte*)&generatorEndIndex, sizeof(generatorEndIndex));
        file.readBytes((byte*)&modulatorEndIndex, sizeof(modulatorEndIndex));

        processPresetGenerators(file, presetIndex, presetZoneIndex, generatorStartIndex, generatorEndIndex);
        processPresetModulators(file, presetIndex, presetZoneIndex, modulatorStartIndex, modulatorEndIndex);
    }
}

void SoundFont2FileReader::processPresetGenerators(FsFile& file, int presetIndex, int presetZoneIndex, 
                                                   uint16_t generatorStartIndex, uint16_t generatorEndIndex) {
    //Serial.println("processPresetGenerators");
    for(int i = generatorStartIndex; i < generatorEndIndex; i++) {
        file.seek(pgenChunk.dataPosition + (PGEN_RECORD_LENGTH*i));

        SoundFont::PresetGenerator generator;
        file.readBytes((byte*)&generator.typeId, sizeof(generator.typeId));
        file.readBytes((byte*)&generator.value, sizeof(generator.value));
        soundFont.addPresetGenerator(presetIndex, presetZoneIndex, generator);
    }
}

void SoundFont2FileReader::processPresetModulators(FsFile& file, int presetIndex, int presetZoneIndex, 
                                                   uint16_t modulatorStartIndex, uint16_t modulatorEndIndex) {
    //Serial.println("processPresetModulators");
    for(int i = modulatorStartIndex; i < modulatorEndIndex; i++) {
        file.seek(pmodChunk.dataPosition + (PMOD_RECORD_LENGTH*i));

        // SoundFont::PresetModulator modulator;
        // file.readBytes((byte*)&modulator.typeId, sizeof(modulator.typeId));
        // file.readBytes((byte*)&modulator.value, sizeof(modulator.value));
        // soundFont.addPresetModulator(presetIndex, presetZoneIndex, modulator);
    }
}

void SoundFont2FileReader::processInstruments(FsFile& file) {
    // Serial.println("processInstruments");
    int instrumentRecords = (instChunk.dataSize / INST_RECORD_LENGTH) - 1;
    for(int i = 0; i < instrumentRecords; i++) {
        file.seek(instChunk.dataPosition + (INST_RECORD_LENGTH*i));

        SoundFont::Instrument instrument;
        file.readBytes((byte*)&instrument.name, sizeof(instrument.name));
        int presetIndex = soundFont.addInstrument(instrument);
        // Serial.print("   ");
        // Serial.println(instrument.name);

        uint16_t zoneStartIndex;
        file.readBytes((byte*)&zoneStartIndex, sizeof(zoneStartIndex));

        uint16_t zoneEndIndex;
        file.seek(instChunk.dataPosition + (INST_RECORD_LENGTH*(i+1)) + 20); //position of zoneIndex in next record
        file.readBytes((byte*)&zoneEndIndex, sizeof(zoneEndIndex));

        processInstrumentZones(file, presetIndex, zoneStartIndex, zoneEndIndex);
    }
}

void SoundFont2FileReader::processInstrumentZones(FsFile& file, int presetIndex, uint16_t zoneStartIndex, uint16_t zoneEndIndex) {
    //Serial.println("processInstrumentZones");
    for(int i = zoneStartIndex; i < zoneEndIndex; i++) {
        int instrumentZoneIndex = soundFont.addInstrumentZone(presetIndex);

        uint16_t generatorStartIndex;
        uint16_t modulatorStartIndex;
        file.seek(ibagChunk.dataPosition + (IBAG_RECORD_LENGTH*i));
        file.readBytes((byte*)&generatorStartIndex, sizeof(generatorStartIndex));
        file.readBytes((byte*)&modulatorStartIndex, sizeof(modulatorStartIndex));

        uint16_t generatorEndIndex;
        uint16_t modulatorEndIndex;
        file.seek(ibagChunk.dataPosition + (IBAG_RECORD_LENGTH*(i+1)));
        file.readBytes((byte*)&generatorEndIndex, sizeof(generatorEndIndex));
        file.readBytes((byte*)&modulatorEndIndex, sizeof(modulatorEndIndex));

        processInstrumentGenerators(file, presetIndex, instrumentZoneIndex, generatorStartIndex, generatorEndIndex);
        processInstrumentModulators(file, presetIndex, instrumentZoneIndex, modulatorStartIndex, modulatorEndIndex);
    }
}

void SoundFont2FileReader::processInstrumentGenerators(FsFile& file, int instrumentIndex, int instrumentZoneIndex, 
                                                       uint16_t generatorStartIndex, uint16_t generatorEndIndex) {
    //Serial.println("processInstrumentGenerators");
    for(int i = generatorStartIndex; i < generatorEndIndex; i++) {
        file.seek(igenChunk.dataPosition + (IGEN_RECORD_LENGTH*i));

        SoundFont::InstrumentGenerator generator;
        file.readBytes((byte*)&generator.typeId, sizeof(generator.typeId));
        file.readBytes((byte*)&generator.value, sizeof(generator.value));
        soundFont.addInstrumentGenerator(instrumentIndex, instrumentZoneIndex, generator);
    }
}

void SoundFont2FileReader::processInstrumentModulators(FsFile& file, int presetIndex, int presetZoneIndex, 
                                                       uint16_t modulatorStartIndex, uint16_t modulatorEndIndex) {
    //Serial.println("processInstrumentModulators");
    for(int i = modulatorStartIndex; i < modulatorEndIndex; i++) {
        file.seek(imodChunk.dataPosition + (IMOD_RECORD_LENGTH*i));

        // SoundFont::InstrumentModulator modulator;
        // file.readBytes((byte*)&modulator.typeId, sizeof(modulator.typeId));
        // file.readBytes((byte*)&modulator.value, sizeof(modulator.value));
        // soundFont.addInstrumentModulator(presetIndex, presetZoneIndex, modulator);
    }
}

void SoundFont2FileReader::processSamples(FsFile& file) {
    // Serial.println("processSamples");
    int sampleRecords = (shdrChunk.dataSize / SHDR_RECORD_LENGTH) - 1;

    for(int i = 0; i < sampleRecords; i++) {
        file.seek(shdrChunk.dataPosition + (SHDR_RECORD_LENGTH*i));

        SoundFont::Sample sample;
        uint32_t start;
        uint32_t end;
        uint32_t sampleRate;
        uint8_t originalPitch;

        file.readBytes((byte*)&sample.name, sizeof(sample.name));
        file.readBytes((byte*)&start, sizeof(start));
        file.readBytes((byte*)&end, sizeof(end));
        file.readBytes((byte*)&sample.startLoop, sizeof(sample.startLoop));
        file.readBytes((byte*)&sample.endLoop, sizeof(sample.endLoop));
        file.readBytes((byte*)&sampleRate, sizeof(sampleRate));
        file.readBytes((byte*)&originalPitch, sizeof(originalPitch));
        file.readBytes((byte*)&sample.pitchCorrection, sizeof(sample.pitchCorrection));
        file.readBytes((byte*)&sample.linkedSample, sizeof(sample.linkedSample));
        file.readBytes((byte*)&sample.sampleType, sizeof(sample.sampleType));

        float sampleFrequency = midiToFrequency(originalPitch);
        sample.sampleBuffer.init(sampleRate, sampleFrequency, end - start, memPool);

        file.seek(smplChunk.dataPosition + start);
        for(size_t i = 0; i < sample.sampleBuffer.getBufferSize(); i++) {
            int16_t intSample;
            file.readBytes((byte*)&intSample, sizeof(intSample));
            // Serial.println(intSample);
            //TODO handle 24 bit samples
            float floatSample = intSample / 32767.0;
            // Serial.println(floatSample);
            sample.sampleBuffer[i] = floatSample;
        }

        soundFont.addSample(sample);
    }
}


            