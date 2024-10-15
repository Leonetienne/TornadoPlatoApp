#include "Test__TextureProjectionWorks.h"
#include "../StaticMeshClasses/Cube.h"
#include "../HandyComponents/Rotator.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"

using namespace Plato;

Test__TextureProjectionWorks::Test__TextureProjectionWorks() :
	Scene(__FUNCTION__) // Set the test fixtures name equal to the specialized class name (constructor function)
{

	mesh_coob = ResourceManager::NewMesh("cube");
    *mesh_coob = Cube();

	txt_coob = ResourceManager::LoadTextureFromBmp("cube", "../furnace.bmp");
	mat_coob = ResourceManager::NewMaterial("cube");
    mat_coob->texture = ResourceManager::FindTexture("cube");

	wo_coob = WorldObjectManager::NewWorldObject();
	wo_coob->AddComponent<Components::MeshRenderer>(mesh_coob, mat_coob);
	wo_coob->AddComponent<Rotator>();

	wo_coob->transform->Move(Vector3d(0, 0, 5));

	return;
}

void Test__TextureProjectionWorks::Update(double frametime)
{
	return;
}
