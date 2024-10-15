#include "Test__TransformHierarchy__ChangeParent.h"
#include "Cube.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"

using namespace Plato;

/*
	Tests that changing an objects parent does not affect its global position

	Press U to parent all objects to a new parent.
	All objects were in a relatively deep transform hierarchy before.
    Press SPACE to apply a local rotation to all joint objects.

	Expected behaviour: The objects do not move, scale, or rotate.
*/

Transform* Test__TransformHierarchy__ChangeParent::CreateLink(Transform* parent)
{
	WorldObject* wo = WorldObjectManager::NewWorldObject("joint", parent);
		wo->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("cube"),
		ResourceManager::FindMaterial("cube")
	);

	wo->AddTag("joint");
	wo->transform->Move(Vector3d::up * 3);
	wo->transform->Rotate(Vector3d::right * 20);
	wo->transform->SetScale(Vector3d(1, 1, 1));

	return wo->transform;
}

Test__TransformHierarchy__ChangeParent::Test__TransformHierarchy__ChangeParent() :
	TestFixture(__FUNCTION__) // Set the test fixtures name equal to the specialized class name (constructor function)
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

	// Create a future parent
	newParent = WorldObjectManager::NewWorldObject("future_parent", nullptr)->transform;
	// Apply some complex transformation to that future parent
	newParent->SetPosition(Vector3d(-15, 33, -99));
	newParent->Rotate(Vector3d(39, -50, 19));
	newParent->SetScale(Vector3d(1, 1, 1));

	// Create a root transform (for positioning and rotation)
	jointRoot = WorldObjectManager::NewWorldObject("joint_root")->transform;
	//jointRoot->Rotate(Quaternion::FromEuler(Vector3d(60, -20, 15)));
	//jointRoot->Scale(Vector3d(10,1,1));

	// Create fifteen joints
	Transform* lastParent = jointRoot;
	for (std::size_t i = 0; i < 15; i++)
		lastParent = CreateLink(lastParent);
	
	system("cls");
	return;
}

void Test__TransformHierarchy__ChangeParent::Update(double frametime)
{
	if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::U))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			//if (wo->GetTransform()->GetParent() != newParent)
				wo->transform->SetParent(newParent);

	// Some fun-control. Will rotate all objects (locally)
	if (Input::Keyboard::GetKey(Input::KEY_CODE::SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->transform->Rotate(Quaternion(Vector3d::right * 0.03));

	if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::R))
		jointRoot->Rotate(Vector3d(0, 0.1, 0));

	return;
}

