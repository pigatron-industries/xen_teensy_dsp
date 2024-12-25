#ifndef SoundFont2FileReader_h
#define SoundFont2FileReader_h

#include "../reader/RiffFileReader.h"
#include "SoundFont.h"

class SoundFont2FileReader : public RiffFileReader {
    public:
        SoundFont2FileReader(SoundFont& soundFont, MemPool<>& memPool);

    protected:
        virtual bool readChunkData(FsFile& file, ChunkHeader chunk);
        virtual void preProcess(FsFile& file);
        virtual void postProcess(FsFile& file);

    private:
        MemPool<>& memPool;
        SoundFont& soundFont;

        ChunkHeader smplChunk;
        ChunkHeader sm24Chunk;
        ChunkHeader phdrChunk;
        ChunkHeader pbagChunk;
        ChunkHeader pmodChunk;
        ChunkHeader pgenChunk;
        ChunkHeader instChunk;
        ChunkHeader ibagChunk;
        ChunkHeader imodChunk;
        ChunkHeader igenChunk;
        ChunkHeader shdrChunk;

        void processPresets(FsFile& file);
        void processPresetZones(FsFile& file, int presetIndex, uint16_t zoneStartIndex, uint16_t zoneEndIndex);
        void processPresetGenerators(FsFile& file, int presetIndex, int presetZoneIndex, uint16_t generatorStartIndex, uint16_t generatorEndIndex);
        void processPresetModulators(FsFile& file, int presetIndex, int presetZoneIndex, uint16_t modulatorStartIndex, uint16_t modulatorEndIndex);

        void processInstruments(FsFile& file);
        void processInstrumentZones(FsFile& file, int presetIndex, uint16_t zoneStartIndex, uint16_t zoneEndIndex);
        void processInstrumentGenerators(FsFile& file, int presetIndex, int presetZoneIndex, uint16_t generatorStartIndex, uint16_t generatorEndIndex);
        void processInstrumentModulators(FsFile& file, int presetIndex, int presetZoneIndex, uint16_t modulatorStartIndex, uint16_t modulatorEndIndex);

        void processSamples(FsFile& file);
};

#endif