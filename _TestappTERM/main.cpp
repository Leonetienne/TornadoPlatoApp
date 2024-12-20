//#include "Test__FPS.h"
#include "../Frontend/TerminalBlockCharacterRenderWindow.h"
#include "../Prefabs/FPSCamera/FPSCameraPrefab.h"
#include "../Scenes/Fun/MC_House/MC_HouseScene.h"
#include "../Scenes/Fun/MC_Furnace/MC_FurnaceScene.h"
#include "../Scenes/Fun/Dust2/Dust2Scene.h"
#include "../Plato/Clock.h"
#include "../Plato/Vector.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Keybinds.h"

using namespace Plato;
using namespace Eule;

int main() {
    // Create a renderer
    const Vector2i resolution(160, 100);
    Renderer renderer(resolution, 0, 1);

    // Create a render window
    TerminalBlockCharacterRenderWindow renderWindow(resolution, "", renderer.GetPixelBuffer());

    // Init the event manager
    Input::EventManager::Init();

    // Instantiate a main camera
    (FPSCameraPrefab(FPSCameraPrefab::CONTROL_TYPE::LOOK_HJKL)).Instantiate();

    // Instantiate the test scene
    Scene* scene = new Dust2Scene;

    // Render the main loop
    Clock frametimer;
    double frametime = 1000.0 / 15.0;
    while (renderWindow.IsRunning()) {
        // Poll the terminal "events"
        renderWindow.PollEvents();

        // Digest plato events
        Input::EventManager::Digest();

        // Delete objects that were flagged to be deleted
        WorldObjectManager::DeleteFlaggedObjects();

        // Tick update hooks
        scene->Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        scene->LateUpdate(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);

        // Render the frame
        if (Input::Keyboard::GetKey(KB_APPPLAYER_QUIT))
            Input::Application::Exit();

        renderer.BeginFrame();
        scene->Render(&renderer);
        WorldObjectManager::CallHook__Render(&renderer);
        renderer.Render();

        // Display the frame
        renderWindow.RedrawWindow();

        // Calculate how long the frame took to display
        frametime = frametimer.GetElapsedTime().AsMilliseconds();
        frametimer.Reset();
    }

    // Free memory (segfaulting on mac qwq)
    WorldObjectManager::Free();
    ResourceManager::Free();
    delete scene;
    scene = nullptr;

    return 0;
}

