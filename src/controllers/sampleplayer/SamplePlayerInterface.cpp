#include "SamplePlayerInterface.h"

void SamplePlayerInterface::init() {
    page.addComponent(&title);

    page.setContext(&Hardware::hw.display);
    page.layout();

    messageBox.setContext(&Hardware::hw.display);
    messageBox.layout();
}

void SamplePlayerInterface::render() {
    Hardware::hw.display.clear();
    page.render();
}

void SamplePlayerInterface::showMessage(const char* message) {
    messageBox.showMessage(message);
}
