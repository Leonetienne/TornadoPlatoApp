//#include "Test__FPS.h"
#include "../Eule/Eule/Vector2.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "Test__FPS.h"
#include "Test__Yubi.h"
#include "Test__Cube.h"
#include <cstring>
#include <iostream>

using namespace Plato;
using namespace Eule;

int main() {
    // Init plato

    // std::cout << "Initializing..." << std::endl;
    Input::EventManager::Init();
    // Create a renderer and a test object
    const Vector2i resolition(50, 50);
    Renderer renderer(resolition);

    // std::cout << "Creating camera..." << std::endl;
    Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
    Components::Camera* camera =
            WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolition, 65, 0.001, 10);

    // Instantiate the test scene
    Test__Cube testScene;

    // Just to be clean, call update once
    // std::cout << "Calling update hooks..." << std::endl;
    WorldObjectManager::CallHook__Update(0.01);
    WorldObjectManager::CallHook__LateUpdate(0.01);
    
    // Render an image
    // std::cout << "Rendering scene..." << std::endl;
    renderer.BeginFrame();
    WorldObjectManager::CallHook__Render(&renderer);
    renderer.Render();

    // Free memory
    // std::cout << "Freeing engine memory..." << std::endl;
    //WorldObjectManager::Free();
    //ResourceManager::Free();

    // Create a bmp image and write the render result to it
    // std::cout << "Initializing bmp image..." << std::endl;
    //BMPlib::BMP bmp(resolition.x, resolition.y, BMPlib::BMP::COLOR_MODE::RGB);
    for (std::size_t y = 0; y < resolition.y; y++) {
        for (std::size_t x = 0; x < resolition.x; x++) {
            const uint8_t* px = renderer.GetPixelBuffer()->GetPixel(Vector2i(x, resolition.y - 1 - y));
            const uint8_t& r = *(px+0);
            const uint8_t& g = *(px+1);
            const uint8_t& b = *(px+2);
            const double bw = r*0.3 + g*0.59 + b*0.11;
            const char asciiCharacter = ".:-=+*%#@"[(int)(((bw / 255.0) * 8) + 0.5)];

            std::cout << "\e[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m" << asciiCharacter << asciiCharacter;
            //bmp.SetPixel(x, y, *(px + 0), *(px + 1), *(px + 2));
        }
        std::cout << std::endl;
    }
    std::cout << "\e[0m";
    std::cout.flush();
    //bmp.Write("render.bmp");
    //std::cout << "Wrote render result as an bmp image! (render.bmp)" << std::endl;

    return 0;
}

