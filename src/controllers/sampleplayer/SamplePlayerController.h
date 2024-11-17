#ifndef SamplePlayerController_h
#define SamplePlayerController_h

#include "Controller.h"
#include "SamplePlayerInterface.h"

#include <Audio.h>


class SamplePlayerController : public Controller {
    public:
        SamplePlayerController() : Controller() {}
        void init(float sampleRate);
        void init();
        void update();
        virtual void process(float **in, float **out, size_t size);


    private:

        SamplePlayerInterface interface;
};

#endif