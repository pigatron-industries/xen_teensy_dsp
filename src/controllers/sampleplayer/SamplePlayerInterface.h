#ifndef SamplePlayerInterface_h
#define SamplePlayerInterface_h

#include <eurorack_graphics.h>
#include "Hardware.h"

class SamplePlayerInterface {
    public:
        SamplePlayerInterface() {}
        void init();
        void render();

        void showMessage(const char* message);

    private:
        FocusManager<TFTDisplay> focusManager;

        VerticalContainer<TFTDisplay, 1> page;
        TextComponent<TFTDisplay> title = TextComponent<TFTDisplay>(TFTDisplay::WIDTH, "SAMPLE PLAYER", 2, TFT_ORANGE);

        MessageBoxComponent<TFTDisplay> messageBox = MessageBoxComponent<TFTDisplay>(TFTDisplay::WIDTH*0.5, TFTDisplay::HEIGHT*0.5, 2, TFT_ORANGE);
};

#endif
