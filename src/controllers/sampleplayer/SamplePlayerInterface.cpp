#include "SamplePlayerInterface.h"

void SamplePlayerInterface::init() {
    page.addComponent(&title);
    page.addComponent(&sampleFile);

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

void SamplePlayerInterface::setSampleFile(const char* filename) {
    sampleFile.setValue(filename);
}

void SamplePlayerInterface::showMessage(const char* message) {
    messageBox.showMessage(message);
}
