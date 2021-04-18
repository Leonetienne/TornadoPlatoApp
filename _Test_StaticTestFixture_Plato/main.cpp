#include "RenderWindow.h"
#include "TestFixture.h"
#include "CameraKeyboardControl.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"

#include "TextureProjectionWorks.h"
#include "TransformHierarchy__Curl.h"

/*
	This project is another testing project for the GameEngine Plato.
	Unlike the other test project "_Test_Plato", which runs dynamic tests, this project is
	for manually running static tests. Tests that would be impractical to do dynamically.

	You can modify the code to launch different predifined test scenarios.
	Use these to manually check if specific things are working
*/

void Loop(TestFixture* tf, Renderer* renderer, RenderWindow* window);

int main()
{
	// Define screen resolution
	const Vector2i resolution(800*2, 600*2);
	

	// Create important objects, such as the Window and the Renderer, including Camera
	RenderWindow window(resolution, "Plato Static Test Fixture");
	Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->GetTransform(); // Necessary for camera rotation
	Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Camera>(resolution, 90, 0.001, 10000);
	cameraYPivot->GetWorldObject()->SetId("main_camera_ypiv");
	camera->worldObject->SetId("main_camera");
	Renderer renderer(resolution);
	renderer.SetMainCamera(camera);
	window.SetPixelBuffer(renderer.GetPixelBuffer());

	// Let's add a CameraKeyboardControl component to the camera by default
	camera->worldObject->AddComponent<CameraKeyboardControl>(cameraYPivot, camera->transform, 0.1, 0.4, 4);

	// Create test fixture. Change that to the fixture you want to use
	TransformHierarchy__Curl testFixture;

	// Adjust window name to test name
	if (testFixture.GetTestName().length() > 0)
		window.SetTitle(testFixture.GetTestName());

	// Poll test fixture whilst window is open
	while (window.GetIsOpen())
		Loop(&testFixture , &renderer, &window);

	// Release memory used by world objects
	WorldObjectManager::Free();
	return 0;
}

void Loop(TestFixture* tf, Renderer* renderer, RenderWindow* window)
{
	WorldObjectManager::DeleteFlaggedObjects();

	// Update test fixture
	tf->Update(1);
	WorldObjectManager::CallHook__Update(1);

	// Clear frame
	renderer->BeginFrame();

	// Render test fixture
	WorldObjectManager::CallHook__Render(renderer);

	// Render frame
	renderer->Render();

	// Update render window pixel buffer
	window->UpdateBgrPixelBuffer();

	return;
}
