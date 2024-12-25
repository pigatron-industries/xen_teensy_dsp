#include "SoundFontManager.h"
#include "SoundFont2FileReader.h"

void SoundFontManager::init() {
    Serial.println("SoundFontManager::init()");
    fs.cd("/");
    FileList& files = fs.ls(".sf2");
    for(int i = 0; i < files.size(); i++) {
        FileInfo& file = files.get(i);
        Serial.println(file.filepath);
    }
}

void SoundFontManager::clear() {
    soundFont.clear();
    memPool.reset();
}

int SoundFontManager::getFileCount() {
    return fs.ls(".sf2").size();
}

FileInfo& SoundFontManager::getFileInfo(int index) {
    fs.cd("/");
    return fs.ls(".sf2").get(index);
}

SoundFont& SoundFontManager::loadSoundFont(int index) {
    clear();

    fs.cd("/");
    FileInfo& file = fs.ls(".sf2").get(index);
    // Serial.print("Loading soundfont: ");
    // Serial.println(file.filepath);
    SoundFont2FileReader fileReader = SoundFont2FileReader(soundFont, memPool);
    fs.read(file.filepath, &fileReader);

    return soundFont;
}