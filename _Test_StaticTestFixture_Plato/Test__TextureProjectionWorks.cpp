#include "Test__TextureProjectionWorks.h"
#include "Cube.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/bmplib.h"
#include "Rotator.h"
#include "Util.h"

Test__TextureProjectionWorks::Test__TextureProjectionWorks() :
	TestFixture(__FUNCTION__), // Set the test fixtures name equal to the specialized class name (constructor function)
	txt_coob(Color::black)
{
	mesh_coob = Cube();
	LoadTextureFromBmp(&txt_coob, "../Plato/Cube_furnace_gitignore_.bmp");
	mat_coob.texture = &txt_coob;

	wo_coob = WorldObjectManager::NewWorldObject();
	wo_coob->AddComponent<MeshRenderer>(&mesh_coob, &mat_coob);
	wo_coob->AddComponent<Rotator>(0.1);

	wo_coob->GetTransform()->Move(Vector3d(0, 0, 5));

	return;
}

void Test__TextureProjectionWorks::Update(double frametime)
{
	return;
}
