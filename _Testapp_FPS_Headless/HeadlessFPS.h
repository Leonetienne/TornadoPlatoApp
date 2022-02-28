#ifndef PLATO_HEADLESSFPS_H
#define PLATO_HEADLESSFPS_H

#include "Test__FPS.h"
#include "CameraKeyboardControl.h"
#include "../Plato/Clock.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Vector.h"
#include <iostream>

// Set your test-class you want to run. Must inherit from TestFixture!
#define TEST_TO_RUN Test__FPS

class HeadlessFPS {
public:
    HeadlessFPS(const Vector2i& resolution);
    ~HeadlessFPS();

    void Loop();
    const TorGL::PixelBuffer<3>* GetRenderPixelBuffer() const;

private:
    Renderer renderer;
    TestFixture* testFixture;
    Clock frametimeClock;
    float curFps = 0;
};

#endif //PLATO_HEADLESSFPS_H
