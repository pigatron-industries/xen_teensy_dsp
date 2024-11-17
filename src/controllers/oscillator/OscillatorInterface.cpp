#include "OscillatorInterface.h"

void OscillatorInterface::init() {
    page.addComponent(&title);

    page.setContext(&Hardware::hw.display);
    page.layout();

    messageBox.setContext(&Hardware::hw.display);
    messageBox.layout();
}

void OscillatorInterface::render() {
    Hardware::hw.display.clear();
    page.render();
}

void OscillatorInterface::showMessage(const char* message) {
    messageBox.showMessage(message);
}
