#include "Test__TransformHierarchy__ClearParent.h"
#include "../StaticMeshClasses/Cube.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"

using namespace Plato;

/*
	Tests that clearing an objects parent does not affect its global position, rotation, or scale.

	Press U to unparent all objects.
    Press SPACE to apply a local rotation to all joint objects.

	Expected behaviour: The objects do not move, scale, or rotate.
    Yes, the ovens are that wiiiiiide.
*/

Transform* Test__TransformHierarchy__ClearParent::CreateLink(Transform* parent)
{
	WorldObject* wo = WorldObjectManager::NewWorldObject("joint", parent);
	wo->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("cube"),
		ResourceManager::FindMaterial("cube")
	);

	wo->AddTag("joint");
	wo->transform->Move(Vector3d::up * 3);
	wo->transform->Rotate(Vector3d::right * 20);

	return wo->transform;
}

Test__TransformHierarchy__ClearParent::Test__TransformHierarchy__ClearParent() :
	Scene(__FUNCTION__) // Set the test fixtures name equal to the specialized class name (constructor function)
{
	// Reposition camera to a more fitting place
	{
		Transform* cam = WorldObjectManager::FindObjectById("main_camera_ypiv")->transform;
		cam->SetPosition(Vector3d(20, -3, 0));
		cam->SetRotation(Quaternion(Vector3d(0, -90, 0)));
	}

	// Create and load assets
	*ResourceManager::NewMesh("cube") = Cube();
	ResourceManager::LoadTextureFromBmp("cube", "../furnace.bmp");
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
	if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::U))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->transform->SetParent(nullptr);

	// Some fun-control. Will rotate all objects (locally)
	if (Input::Keyboard::GetKey(Input::KEY_CODE::SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->transform->Rotate(Quaternion(Vector3d::right * 0.03));
	
	if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::R))
		jointRoot->Rotate(Vector3d(0, 0.1, 0));

	return;
}

