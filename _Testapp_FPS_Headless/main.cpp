//#include "Test__FPS.h"
#include "../Eule/Eule/Vector2.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "Test__FPS.h"
#include <cstring>
#include <iostream>

using namespace Plato;
using namespace Eule;

int main() {
    // Init plato

    std::cout << "Initializing..." << std::endl;
    Input::EventManager::Init();
    // Create a renderer and a test object
    const Vector2i resolition(800, 600);
    Renderer renderer(resolition);

    std::cout << "Creating camera..." << std::endl;
    Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
    Components::Camera* camera =
            WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolition, 90, 0.001, 10);

    // Instantiate the test scene
    Test__FPS testScene;

    // Just to be clean, call update once
    std::cout << "Calling update hooks..." << std::endl;
    for (int i = 0; i < 120; i++) {
        Input::EventManager::RegisterEventKeyDown(Input::KEY_CODE::S);

        WorldObjectManager::CallHook__Update(0.01);
        WorldObjectManager::CallHook__LateUpdate(0.01);

        Input::EventManager::RegisterEventKeyUp(Input::KEY_CODE::S);
    }
    
    // Render an image
    std::cout << "Rendering scene..." << std::endl;
    renderer.BeginFrame();
    WorldObjectManager::CallHook__Render(&renderer);
    renderer.Render();

    // Free memory
    std::cout << "Freeing engine memory..." << std::endl;
    WorldObjectManager::Free();
    ResourceManager::Free();

    // Create a bmp image and write the render result to it
    std::cout << "Initializing bmp image..." << std::endl;
    BMPlib::BMP bmp(resolition.x, resolition.y, BMPlib::BMP::COLOR_MODE::RGB);
    for (std::size_t y = 0; y < bmp.GetHeight(); y++) {
        for (std::size_t x = 0; x < bmp.GetWidth(); x++) {
            const uint8_t* px = renderer.GetPixelBuffer()->GetPixel(Vector2i(x, bmp.GetHeight() - 1 - y));

            bmp.SetPixel(x, y, *(px + 0), *(px + 1), *(px + 2));
        }
    }
    bmp.Write("render.bmp");
    std::cout << "Wrote render result as an bmp image! (render.bmp)" << std::endl;

    return 0;
}

