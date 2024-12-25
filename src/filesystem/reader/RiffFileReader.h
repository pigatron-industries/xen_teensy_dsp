#ifndef RiffFileReader_h
#define RiffFileReader_h

#include <eurorack_sd.h>

class ChunkHeader {
    public:
        uint32_t position;
        char id[4];
        uint32_t dataPosition;
        uint32_t dataSize;
};

class RiffFileReader : public FileReader {
    public:
        RiffFileReader(char* format);
        bool read(FsFile& file);

    protected:
        virtual bool readChunkData(FsFile& file, ChunkHeader chunk);
        virtual void readRiffChunk(FsFile& file, ChunkHeader chunk);
        virtual void readListChunk(FsFile& file, ChunkHeader chunk);
        virtual void preProcess(FsFile& file) {};
        virtual void postProcess(FsFile& file) {};

        void readSubChunks(FsFile& file, ChunkHeader chunk);

    private:
        char* format;

        void readChunk(FsFile& file);
        ChunkHeader readChunkHeader(FsFile& file);
        
};

#endif