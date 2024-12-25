#ifndef SoundFontManager_h
#define SoundFontManager_h

#include <eurorack.h>
#include <eurorack_sd.h>
#include "SoundFont.h"


class SoundFontManager {
    public:
        SoundFontManager(FileSystem& fs, MemPool<>& memPool) : fs(fs), memPool(memPool) {}
        void init();
        void clear();
        int getFileCount();
        FileInfo& getFileInfo(int index);
        SoundFont& loadSoundFont(int index);

    private:
        FileSystem& fs;

        // only one soundfont at a time
        SoundFont soundFont;

        // memory management
        MemPool<>& memPool;

};

#endif