#ifndef OscillatorInterface_h
#define OscillatorInterface_h

#include <eurorack_graphics.h>
#include "Hardware.h"

class OscillatorInterface {
    public:
        OscillatorInterface() {}
        void init();
        void render();

        void showMessage(const char* message);

    private:
        FocusManager<TFTDisplay> focusManager;

        VerticalContainer<TFTDisplay, 1> page;
        TextComponent<TFTDisplay> title = TextComponent<TFTDisplay>(TFTDisplay::WIDTH, "OSCILLATOR", 2, TFT_ORANGE);

        MessageBoxComponent<TFTDisplay> messageBox = MessageBoxComponent<TFTDisplay>(TFTDisplay::WIDTH*0.5, TFTDisplay::HEIGHT*0.5, 2, TFT_ORANGE);
};

#endif
