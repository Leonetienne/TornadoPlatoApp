#include "../Frontend/SDL2RenderWindow.h"
#include "../HandyComponents/CameraFPSKeyboardControl.h"
#include "../Plato/Clock.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Vector.h"
#include "../Plato/Scene.h"
#include <iostream>

// Include test cases
#include "Test__TransformHierarchy__Curl.h"
#include "Test__TransformHierarchy__ClearParent.h"
#include "Test__TransformHierarchy__ChangeParent.h"
#include "Test__OBJParser__Load.h"
#include "Test__OBJParser__LoadMultimesh.h"
#include "Test__FPS.h"
#include "Test__Lighting__PointLight.h"

// Set your test-class you want to run. Must inherit from TestFixture!
#define TEST_TO_RUN Test__FPS

/*
	This project is another testing project for the GameEngine Plato.
	Unlike the other test project "_Test_Plato", which runs dynamic tests, this project is
	for manually running static tests. Tests that would be impractical to do dynamically.

	You can modify the code to launch different predifined test scenarios.
	Use these to manually check if specific things are working
*/

using namespace Plato;
using Input::EventManager;
using Input::REVERSE_EVENT_CALLBACK;

Clock frameTimeClock;

void Loop(Scene* scene, Renderer* renderer, RenderWindow* window);
void RegisterReverseEventCallbacks(RenderWindow* window);

int main()
{
	// Initialize event manager
	EventManager::Init();

	// Define screen resolution
	const Vector2i resolution = (Vector2d(800, 600) * 2).ToInt();
	

	// Create important objects, such as the Window and the Renderer, including Camera
	Renderer renderer(resolution, 0, 0.8);
	SDL2RenderWindow renderWindow(resolution, "Plato Static Test Fixture", renderer.GetPixelBuffer());
    renderWindow.EnableMouseCameraControlMode();
	Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
	Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)
        ->AddComponent<Components::Camera>(
            90,
            0.001,
            10
    );
	cameraYPivot->worldObject->SetId("main_camera_ypiv");
	camera->SetAsMainCamera();

	// Register reverse-event callbacks
	RegisterReverseEventCallbacks(&renderWindow);

	// Let's add a CameraKeyboardControl component to the camera by default
    camera->worldObject->AddComponent<CameraFPSKeyboardControl>(
        cameraYPivot,
        camera->transform,
        0.1,
        2,
        4
    );

	// Create test fixture. Change that to the fixture you want to use (in the macro definition)
	TEST_TO_RUN scene;

	// Adjust window name to test name
	if (scene.GetSceneName().length() > 0)
        renderWindow.SetWindowTitle(scene.GetSceneName());

	// Poll test fixture whilst window is open
    while (renderWindow.IsRunning())
		Loop(&scene , &renderer, &renderWindow);

	// Release memory
	WorldObjectManager::Free();
	ResourceManager::Free();
	return 0;
}

void Loop(Scene* scene, Renderer* renderer, RenderWindow* window)
{
	// Get elapsed time for last frame
	const double elapsedTime = frameTimeClock.GetElapsedTime().AsMilliseconds();
	
	// Reset frametime clock
	frameTimeClock.Reset();

    // Poll RenderWindow events
    window->PollEvents();

	// Digest events
	Input::EventManager::Digest();

    // Delete flagged objects
	WorldObjectManager::DeleteFlaggedObjects();

	// Update test fixture
    if (Input::Keyboard::GetKey(Input::KEY_CODE::ESCAPE))
      Input::Application::Exit();

	scene->Update(elapsedTime);
	WorldObjectManager::CallHook__Update(elapsedTime);
	scene->LateUpdate(elapsedTime);
	WorldObjectManager::CallHook__LateUpdate(elapsedTime);

	// Clear frame
	renderer->BeginFrame();

	// Render test fixture
	scene->Render(renderer);
	WorldObjectManager::CallHook__Render(renderer);

	// Render frame
	renderer->Render();

	// Update render window pixel buffer
	window->RedrawWindow();


	// Add fps to title
	std::stringstream ss;
	ss << scene->GetSceneName() << " - FPS: " << (int)(1000.0 / elapsedTime);
	window->SetWindowTitle(ss.str());

	// Limit fps to 120
	//std::this_thread::sleep_for(std::chrono::milliseconds((long long)
	//	std::max<double>(((1000.0/120) - frameTimeClock.GetElapsedTime().AsMilliseconds()), 0)
	//));

	return;
}

void RegisterReverseEventCallbacks(RenderWindow* window)
{
	// Callback to exit application
	EventManager::RegisterReverseEventCallback(
		REVERSE_EVENT_CALLBACK::EXIT,
		[window](const std::vector<double>& params)
		{
			window->Close();
			return;
		}
	);

	return;
}

