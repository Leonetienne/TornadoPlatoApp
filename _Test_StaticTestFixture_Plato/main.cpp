#include "RenderWindow.h"
#include "TestFixture.h"
#include "CameraKeyboardControl.h"
#include "../Plato/Clock.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
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

Clock frameTimeClock;
void Loop(TestFixture* tf, Renderer* renderer, RenderWindow* window);

int main()
{
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

	// Add fps to title
	std::stringstream ss;
	ss << tf->GetTestName() << " - FPS: " << (int)(1000.0 / elapsedTime);
	window->SetTitle(ss.str());

	return;
}
