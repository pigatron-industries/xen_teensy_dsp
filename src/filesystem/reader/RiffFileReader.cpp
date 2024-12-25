#include "RiffFileReader.h"
#include "util/stringutil.h"

RiffFileReader::RiffFileReader(char* format) {
    this->format = format;
}

bool RiffFileReader::read(FsFile& file) {
    preProcess(file);
    readChunk(file);
    postProcess(file);
    return true;
}

void RiffFileReader::readChunk(FsFile& file) {
    ChunkHeader chunk = readChunkHeader(file);
    readChunkData(file, chunk);
    file.seek(chunk.position + 8 + chunk.dataSize);
}

ChunkHeader RiffFileReader::readChunkHeader(FsFile& file) {
    ChunkHeader chunk;
    chunk.position = file.position();
    chunk.dataPosition = chunk.position + 8;
    
    file.readBytes(chunk.id, 4);

    uint8_t size[4];
    file.readBytes((byte*)&chunk.dataSize, 4);

    // Serial.write(chunk.id, 4);
    // Serial.print(" ");
    // Serial.print(chunk.position);
    // Serial.print(" ");
    // Serial.println(chunk.dataSize);

    return chunk;
}

bool RiffFileReader::readChunkData(FsFile& file, ChunkHeader chunk) {
    if(beginsWith(chunk.id, "RIFF")) {
        readRiffChunk(file, chunk);
        return true;
    } else if(beginsWith(chunk.id, "LIST")) {
        readListChunk(file, chunk);
        return true;
    } else {
        return false;
    }
}

void RiffFileReader::readSubChunks(FsFile& file, ChunkHeader container) {
    uint32_t end = container.dataPosition + container.dataSize;
    while(file.position() < end) {
        readChunk(file);
    }
}

void RiffFileReader::readRiffChunk(FsFile& file, ChunkHeader chunk) {
    file.readBytes(format, 4);
    // Serial.print("   ");
    // Serial.write(format, 4);
    // Serial.println();
    readSubChunks(file, chunk);
}

void RiffFileReader::readListChunk(FsFile& file, ChunkHeader chunk) {
    char type[4];
    file.readBytes(type, 4);
    // Serial.print("   ");
    // Serial.write(type, 4);
    // Serial.println();
    readSubChunks(file, chunk);
}
