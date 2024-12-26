#include "SamplePlayerInterface.h"

void SamplePlayerInterface::init() {
    page.addComponent(&title);
    page.addComponent(&sampleFile);
    page.addComponent(&presetField);
    page.addComponent(&modeField);

    page.setContext(&Hardware::hw.display);
    page.layout();

    messageBox.setContext(&Hardware::hw.display);
    messageBox.layout();
}

void SamplePlayerInterface::render() {
    Hardware::hw.display.clear();
    page.render();
}

void SamplePlayerInterface::focusSampleFile() {
    focusManager.setFocus(&sampleFile);
}

void SamplePlayerInterface::focusPreset() {
    focusManager.setFocus(&presetField);
}

void SamplePlayerInterface::focusMode() {
    focusManager.setFocus(&modeField);
}

void SamplePlayerInterface::setSampleFile(const char* filename) {
    sampleFile.setValue(filename);
}

void SamplePlayerInterface::setPreset(int preset) {
    presetField.setValue(preset);
}

void SamplePlayerInterface::setMode(int mode) {
    modeField.setValue(mode);
}

void SamplePlayerInterface::showMessage(const char* message) {
    messageBox.showMessage(message);
}
