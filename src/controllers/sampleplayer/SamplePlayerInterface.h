#ifndef SamplePlayerInterface_h
#define SamplePlayerInterface_h

#include <eurorack_graphics.h>
#include "Hardware.h"

class SamplePlayerInterface {
    public:
        SamplePlayerInterface() {}
        void init();
        void render();

        void focusSampleFile();
        void focusPreset();
        void focusMode();
        void setSampleFile(const char* filename);
        void setPreset(int preset);
        void setMode(int mode);

        void showMessage(const char* message);

    private:
        FocusManager<TFTDisplay> focusManager;

        VerticalContainer<TFTDisplay, 4> page;
        TextComponent<TFTDisplay> title = TextComponent<TFTDisplay>(TFTDisplay::WIDTH, "SAMPLE PLAYER", 2, TFT_ORANGE);
        FieldComponent<TFTDisplay> sampleFile = FieldComponent<TFTDisplay>(TFTDisplay::WIDTH, 50, "FILE:");
        FieldComponent<TFTDisplay> presetField = FieldComponent<TFTDisplay>(TFTDisplay::WIDTH, 50, "PRESET:");
        FieldComponent<TFTDisplay> modeField = FieldComponent<TFTDisplay>(TFTDisplay::WIDTH, 50, "MODE:");

        MessageBoxComponent<TFTDisplay> messageBox = MessageBoxComponent<TFTDisplay>(TFTDisplay::WIDTH*0.5, TFTDisplay::HEIGHT*0.5, 2, TFT_ORANGE);
};

#endif
