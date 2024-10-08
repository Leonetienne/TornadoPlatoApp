//#include "Test__FPS.h"
#include "../Eule/Eule/Vector2.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "TestFixture.h"
#include "Test__FPS.h"
#include "Test__Yubi.h"
#include "Test__Cube.h"
#include "Test__MC.h"
#include "CameraKeyboardControl.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <chrono>

using namespace Plato;
using namespace Eule;

// Function to set terminal mode to non-canonical
void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to reset terminal mode
void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Re-enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to check if a key has been pressed (non-blocking)
bool keyPressed() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

// Function to get a key press
int getKey() {
    if (keyPressed()) {
        char ch;
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }
    return -1;
}


void Loop(Renderer& renderer, Vector2i resolution, TestFixture& fix) {
    static double frametime = 1.0/60.0;

    // Get the starting time point
    auto start = std::chrono::high_resolution_clock::now();

    // Handle key presses
    const char key = getKey();

    if (key != -1) {
        Input::EventManager::RegisterEventKeyDown((Input::KEY_CODE)key);
        //std::cout << (int)key << std::endl;
        //exit(0);
    }
    Input::EventManager::Digest();

    WorldObjectManager::DeleteFlaggedObjects();

    // Just to be clean, call update once
    // std::cout << "Calling update hooks..." << std::endl;
    fix.Update(frametime);
    WorldObjectManager::CallHook__Update(frametime);
    WorldObjectManager::CallHook__LateUpdate(frametime);
    
    // Render an image
    // std::cout << "Rendering scene..." << std::endl;
    renderer.BeginFrame();
    WorldObjectManager::CallHook__Render(&renderer);
    renderer.Render();

    // Free memory (segfaulting on mac qwq)
    // std::cout << "Freeing engine memory..." << std::endl;
    //WorldObjectManager::Free();
    //ResourceManager::Free();

    // Draw image to console
    std::cout << "\e[2J\e[H";

    for (std::size_t y = 0; y < resolution.y; y++) {
        for (std::size_t x = 0; x < resolution.x; x++) {
            const uint8_t* px = renderer.GetPixelBuffer()->GetPixel(Vector2i(x, resolution.y - 1 - y));
            const uint8_t& r = *(px+0);
            const uint8_t& g = *(px+1);
            const uint8_t& b = *(px+2);
            //const double bw = r*0.3 + g*0.59 + b*0.11;
            //const char asciiCharacter = ".:-=+*%#@"[(int)(((bw / 255.0) * 8) + 0.5)];

            std::cout << "\e[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m" << "\u2588\u2588";
            //std::cout << "\e[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m" << asciiCharacter << asciiCharacter;
        }
        std::cout << std::endl;
    }
    std::cout << "\e[0m";
    std::cout.flush();
    usleep(15000);

	// Digest events
    if (key != -1) {
        Input::EventManager::RegisterEventKeyUp((Input::KEY_CODE)key);
    }
	Input::EventManager::Digest();

    // Get the ending time point
    auto end = std::chrono::high_resolution_clock::now();
    frametime = std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    // Init plato
    enableRawMode();

    // std::cout << "Initializing..." << std::endl;
    Input::EventManager::Init();
    // Create a renderer and a test object
    const Vector2i resolution(160, 100);
    Renderer renderer(resolution);

    // std::cout << "Creating camera..." << std::endl;
	Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
	Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolution, 90, 0.001, 10);
	cameraYPivot->worldObject->SetId("main_camera_ypiv");
	camera->SetAsMainCamera();
	// Let's add a CameraKeyboardControl component to the camera by default
	camera->worldObject->AddComponent<CameraKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);

    // Instantiate the test scene
    Test__MC testScene;

    while (1) {
        Loop(renderer, resolution, testScene);
    }

    return 0;
}

