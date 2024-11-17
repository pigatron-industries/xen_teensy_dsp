#include "Hardware.h"

Hardware Hardware::hw = Hardware();

EXTMEM uint8_t Hardware::memPoolSamplesBuffer[MEMPOOL_SAMPLES_SIZE];

void Hardware::init() {
    NativeDevice::instance.init();

    display.init();
    fsSamples.init();

    AudioMemory(10);
    Hardware::hw.sgtl5000_1.enable();
    Hardware::hw.sgtl5000_1.volume(0.3);
}
