#include "Test__TransformHierarchy__ClearParent.h"
#include "Cube.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/bmplib.h"
#include "../Plato/ResourceManager.h"
#include "Rotator.h"
#include "Util.h"

/*
	Tests that clearing an objects parent does not affect its global position

	Press U to unparent all objects.

	Expected behaviour: The objects do not move, scale, or rotate.
*/

Transform* Test__TransformHierarchy__ClearParent::CreateLink(Transform* parent)
{
	WorldObject* wo = WorldObjectManager::NewWorldObject("joint", parent);
	wo->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("cube"),
		ResourceManager::FindMaterial("cube")
	);

	wo->AddTag("joint");
	wo->transform->Move(Vector3d::up * 3);
	wo->transform->Rotate(Vector3d::right * 20);

	return wo->transform;
}

Test__TransformHierarchy__ClearParent::Test__TransformHierarchy__ClearParent() :
	TestFixture(__FUNCTION__) // Set the test fixtures name equal to the specialized class name (constructor function)
{
	// Reposition camera to a more fitting place
	{
		Transform* cam = WorldObjectManager::FindObjectById("main_camera_ypiv")->transform;
		cam->SetPosition(Vector3d(20, -3, 0));
		cam->SetRotation(Quaternion::FromEuler(Vector3d(0, -90, 0)));
	}

	// Create and load assets
	*ResourceManager::NewMesh("cube") = Cube();
	ResourceManager::LoadTextureFromBmp("cube", "../Plato/Cube_furnace_gitignore_.bmp");
	ResourceManager::NewMaterial("cube")->texture = ResourceManager::FindTexture("cube");

	// Create a root transform (for positioning and rotation)
	jointRoot = WorldObjectManager::NewWorldObject("joint_root")->transform;
	jointRoot->Scale(Vector3d(10,1,1));
	//jointRoot->Rotate(Quaternion::FromEuler(Vector3d(60, -20, 15)));

	// Create fifteen joints
	Transform* lastParent = jointRoot;
	for (std::size_t i = 0; i < 15; i++)
		lastParent = CreateLink(lastParent);

	return;
}

void Test__TransformHierarchy__ClearParent::Update(double frametime)
{
	if (GetAsyncKeyState('U'))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->transform->SetParent(nullptr);

	// Some fun-control. Will rotate all objects (locally)
	if (GetAsyncKeyState(VK_SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->transform->Rotate(Quaternion::FromEuler(Vector3d::right * 0.03));
	
	if (GetAsyncKeyState('R'))
		jointRoot->Rotate(Vector3d(0, 0.1, 0));

	return;
}

