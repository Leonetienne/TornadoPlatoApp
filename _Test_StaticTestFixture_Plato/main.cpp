#include "RenderWindow.h"
#include "TestFixture.h"
#include "TextureProjectionWorks.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"

/*
	This project is another testing project for the GameEngine Plato.
	Unlike the other test project "_Test_Plato", which runs dynamic tests, this project is
	for manually running static tests. Tests that would be impractical to do dynamically.

	You can modify the code to launch different predifined test scenarios.
	Use these to manually check if specific things are working
*/

void Loop(TestFixture* tf, Renderer* renderer);

int main()
{
	// Define screen resolution
	const Vector2i resolution(800*2, 600*2);
	

	// Create important objects, such as the Window and the Renderer including Camera
	RenderWindow window(resolution, "Plato Static Test Fixture");
	Camera* camera = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(resolution, 90, 0.001, 10000);
	Renderer renderer(resolution);
	renderer.SetMainCamera(camera);
	window.SetPixelBuffer(renderer.GetPixelBuffer());

	// Create test fixture. Change that to the fixture you want to use
	TextureProjectionWorks testFixture;

	// Adjust window name to test name
	if (testFixture.GetTestName().length() > 0)
		window.SetTitle(testFixture.GetTestName());

	// Poll window and test fixture
	while (window.GetIsOpen())
	{
		window.Poll();
		Loop(&testFixture , &renderer);
	}

	// Release memory used by world objects
	WorldObjectManager::Free();
	return 0;
}

void Loop(TestFixture* tf, Renderer* renderer)
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

	return;
}
