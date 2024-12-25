#include "SamplePlayerController.h"

#define LEFT 0
#define RIGHT 1


void SamplePlayerController::init(float sampleRate) {
    Controller::init(sampleRate);
    player.init(sampleRate);
    configParam(Parameter::SAMPLEFILE, 0, Hardware::hw.soundFontManager.getFileCount()-1, false);
    interface.init();
    interface.focusSampleFile();
    init();

    if(Hardware::hw.soundFontManager.getFileCount() == 0) {
        interface.showMessage("No samples");
    } else {
        loadSampleFile(parameters[Parameter::SAMPLEFILE].value);
    }
}

void SamplePlayerController::init() {
    Serial.println("SamplePlayerController::init");
    interface.render();
}


int SamplePlayerController::cycleParameter(int amount) {
    parameters.cycle(amount);

    switch(parameters.getSelectedIndex()) {
        case Parameter::SAMPLEFILE:
            interface.focusSampleFile();
            break;
    }

    return parameters.getSelectedIndex(); 
}


void SamplePlayerController::cycleValue(int amount) {
    int value = parameters.getSelected().cycle(amount);
    switch(parameters.getSelectedIndex()) {
        case Parameter::SAMPLEFILE: {
            FileInfo& file = Hardware::hw.soundFontManager.getFileInfo(value);
            interface.setSampleFile(file.filename);
            break;
        }
    }

    save();
}


void SamplePlayerController::selectValue() {
    int prevValue = parameters.getSelected().getValue();
    int newValue = parameters.getSelected().select();
    switch(parameters.getSelectedIndex()) {
        case Parameter::SAMPLEFILE:
            if (newValue != prevValue) {
                loadSampleFile(newValue);
            } else if(soundFont != nullptr) {
                interface.setSampleFile(soundFont->getName());
            }
            break;
    }
}


void SamplePlayerController::loadSampleFile(int index) {
    Serial.println("SamplePlayerController::loadSampleFile");
    interface.showMessage("Loading");
    soundFont = &Hardware::hw.soundFontManager.loadSoundFont(index);

    soundFont->print();
    
    interface.setSampleFile(soundFont->getName());
    interface.render();

    player.setSoundFont(soundFont, 0); //TODO select preset
}


void SamplePlayerController::update() {
    
    if(triggerInput.update()) {
        if(triggerInput.isTriggeredOn()) {
            pitchInput.update();
            velocityInput.update();
            player.trigger(velocityInput.getValue(), pitchInput.getValue());
        }
    }
    
}

void SamplePlayerController::process(float **in, float **out, size_t size) {
    for (size_t i = 0; i < size; i++) {
        player.process();

        out[LEFT][i] = player.getOutput()[LEFT];
        out[RIGHT][i] = player.getOutput()[RIGHT];
    }
}
