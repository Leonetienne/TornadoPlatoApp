#include "RenderWindow.h"
#include "TestFixture.h"
#include "CameraKeyboardControl.h"
#include "../Plato/Clock.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include <iostream>

// Include test cases
#include "Test__TransformHierarchy__Curl.h"
#include "Test__TransformHierarchy__ClearParent.h"
#include "Test__TransformHierarchy__ChangeParent.h"
#include "Test__OBJParser__Load.h"
#include "Test__OBJParser__LoadMultimesh.h"

// Set your test-class you want to run. Must inherit from TestFixture!
#define TEST_TO_RUN Test__OBJParser__LoadMultimesh

/*
	This project is another testing project for the GameEngine Plato.
	Unlike the other test project "_Test_Plato", which runs dynamic tests, this project is
	for manually running static tests. Tests that would be impractical to do dynamically.

	You can modify the code to launch different predifined test scenarios.
	Use these to manually check if specific things are working
*/

using Input::EventManager;
using Input::REVERSE_EVENT_CALLBACK;

Clock frameTimeClock;
void Loop(TestFixture* tf, Renderer* renderer, RenderWindow* window);

void RegisterReverseEventCallbacks(RenderWindow* window);

int main()
{
	// Initialize event manager
	EventManager::Init();

	// Define screen resolution
	const Vector2i resolution = Vector2i(800, 600) * 2;
	

	// Create important objects, such as the Window and the Renderer, including Camera
	RenderWindow window(resolution, "Plato Static Test Fixture");
	Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
	Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Camera>(resolution, 90, 0.001, 10000);
	cameraYPivot->worldObject->SetId("main_camera_ypiv");
	camera->worldObject->SetId("main_camera");
	Renderer renderer(resolution);
	renderer.SetMainCamera(camera);
	window.SetPixelBuffer(renderer.GetPixelBuffer());

	// Register reverse-event callbacks
	RegisterReverseEventCallbacks(&window);

	// Let's add a CameraKeyboardControl component to the camera by default
	camera->worldObject->AddComponent<CameraKeyboardControl>(cameraYPivot, camera->transform, 0.4, 0.6, 4);

	// Create test fixture. Change that to the fixture you want to use (in the macro definition)
	TEST_TO_RUN testFixture;

	// Adjust window name to test name
	if (testFixture.GetTestName().length() > 0)
		window.SetTitle(testFixture.GetTestName());

	// Poll test fixture whilst window is open
	while (window.GetIsOpen())
		Loop(&testFixture , &renderer, &window);

	// Release memory
	WorldObjectManager::Free();
	ResourceManager::Free();
	return 0;
}

void Loop(TestFixture* tf, Renderer* renderer, RenderWindow* window)
{
	// Get elapsed time for last frame
	const double elapsedTime = frameTimeClock.GetElapsedTime().AsMilliseconds();
	
	// Reset frametime clock
	frameTimeClock.Reset();

	WorldObjectManager::DeleteFlaggedObjects();

	// Update test fixture
	tf->Update(elapsedTime);
	WorldObjectManager::CallHook__Update(elapsedTime);

	// Clear frame
	renderer->BeginFrame();

	// Render test fixture
	WorldObjectManager::CallHook__Render(renderer);

	// Render frame
	renderer->Render();

	// Update render window pixel buffer
	window->UpdateBgrPixelBuffer();

	// Digest events
	Input::EventManager::Digest();

	// Add fps to title
	std::stringstream ss;
	ss << tf->GetTestName() << " - FPS: " << (int)(1000.0 / elapsedTime);
	window->SetTitle(ss.str());

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

	// Callback to set global mouse position
	Input::EventManager::RegisterReverseEventCallback(
		Input::REVERSE_EVENT_CALLBACK::SET_GLOBAL_MOUSE_POSITION,
		[](const std::vector<double>& params)
		{
			const int x = params[0];
			const int y = params[1];
			SetCursorPos(x, y);
			return;
		}
	);

	// Callback to set local mouse position
	EventManager::RegisterReverseEventCallback(
		REVERSE_EVENT_CALLBACK::SET_LOCAL_MOUSE_POSITION,
		[window](const std::vector<double>& params)
		{
			POINT p;
			p.x = params[0];
			p.y = params[1];

			// Translate relative position to global position
			ClientToScreen(window->GetSystemWindowHandle(), &p);

			// Set global position
			SetCursorPos(p.x, p.y);

			return;
		}
	);

	return;
}
