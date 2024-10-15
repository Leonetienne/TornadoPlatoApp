#include "Test__TextureProjectionWorks.h"
#include "Cube.h"
#include "Util.h"
#include "../HandyComponents/Rotator.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/Color.h"

using namespace Plato;

Test__TextureProjectionWorks::Test__TextureProjectionWorks() :
	TestFixture(__FUNCTION__), // Set the test fixtures name equal to the specialized class name (constructor function)
	txt_coob(Color::black)
{
	mesh_coob = Cube();
	LoadTextureFromBmp(&txt_coob, "../furnace.bmp");
	mat_coob.texture = &txt_coob;

	wo_coob = WorldObjectManager::NewWorldObject();
	wo_coob->AddComponent<Components::MeshRenderer>(&mesh_coob, &mat_coob);
	wo_coob->AddComponent<Rotator>();

	wo_coob->transform->Move(Vector3d(0, 0, 5));

	return;
}

void Test__TextureProjectionWorks::Update(double frametime)
{
	return;
}
