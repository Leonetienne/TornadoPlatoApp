//#include "Test__FPS.h"
#include "../Frontend/TerminalBlockCharacterRenderWindow.h"
#include "../HandyComponents/CameraExclKeyboardControl.h"
#include "../Plato/Clock.h"
#include "../Plato/Vector.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "TestFixture.h"
#include "Test__FPS.h"
#include "Test__Yubi.h"
#include "Test__Cube.h"
#include "Test__MC.h"
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace Plato;
using namespace Eule;


int main() {
    // Create a renderer
    const Vector2i resolution(160, 100);
    Renderer renderer(resolution, 0, 0);

    // Create a render window
    TerminalBlockCharacterRenderWindow renderWindow(resolution, "", renderer.GetPixelBuffer());

    // Init the event manager
    Input::EventManager::Init();

	Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
	Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(90, 0.001, 10);
	cameraYPivot->worldObject->SetId("main_camera_ypiv");
	camera->SetAsMainCamera();
	// Let's add a CameraKeyboardControl component to the camera by default
	camera->worldObject->AddComponent<CameraExclKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);

    // Instantiate the test scene
    Test__MC testScene;

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
        testScene.Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);

        // Render the frame
        renderer.BeginFrame();
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

    return 0;
}

