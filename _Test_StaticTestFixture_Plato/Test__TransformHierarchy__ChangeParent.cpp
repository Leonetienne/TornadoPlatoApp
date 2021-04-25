#include "Test__TransformHierarchy__ChangeParent.h"
#include "Cube.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/bmplib.h"
#include "../Plato/ResourceManager.h"
#include "Rotator.h"
#include "Util.h"

/*
	Tests that changing an objects parent does not affect its global position

	Press U to parent all objects to a new parent.
	All objects were in a relatively deep transform hierarchy before.

	Expected behaviour: The objects do not move, scale, or rotate.
*/

Transform* Test__TransformHierarchy__ChangeParent::CreateLink(Transform* parent)
{
	WorldObject* wo = WorldObjectManager::NewWorldObject("joint", parent);
		wo->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("cube"),
		ResourceManager::FindMaterial("cube")
	);

	wo->AddTag("joint");
	wo->GetTransform()->Move(Vector3d::up * 3);
	wo->GetTransform()->Rotate(Vector3d::right * 20);
	wo->GetTransform()->SetScale(Vector3d(1, 1, 1));

	return wo->GetTransform();
}

Test__TransformHierarchy__ChangeParent::Test__TransformHierarchy__ChangeParent() :
	TestFixture(__FUNCTION__) // Set the test fixtures name equal to the specialized class name (constructor function)
{
	// Reposition camera to a more fitting place
	{
		Transform* cam = WorldObjectManager::FindObjectById("main_camera_ypiv")->GetTransform();
		cam->SetPosition(Vector3d(20, -3, 0));
		cam->SetRotation(Quaternion::FromEuler(Vector3d(0, -90, 0)));
	}

	// Create and load assets
	*ResourceManager::NewMesh("cube") = Cube();
	ResourceManager::LoadTextureFromBmp("cube", "../Plato/Cube_furnace_gitignore_.bmp");
	ResourceManager::NewMaterial("cube")->texture = ResourceManager::FindTexture("cube");

	// Create a future parent
	newParent = WorldObjectManager::NewWorldObject("future_parent", nullptr)->GetTransform();
	// Apply some complex transformation to that future parent
	newParent->SetPosition(Vector3d(-15, 33, -99));
	newParent->Rotate(Vector3d(39, -50, 19));
	newParent->SetScale(Vector3d(1, 1, 1));

	// Create a root transform (for positioning and rotation)
	jointRoot = WorldObjectManager::NewWorldObject("joint_root")->GetTransform();
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
	if (GetAsyncKeyState('U'))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			//if (wo->GetTransform()->GetParent() != newParent)
				wo->GetTransform()->SetParent(newParent);

	// Some fun-control. Will rotate all objects (locally)
	if (GetAsyncKeyState(VK_SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->GetTransform()->Rotate(Quaternion::FromEuler(Vector3d::right * 0.03));

	if (GetAsyncKeyState('R'))
		jointRoot->Rotate(Vector3d(0, 0.1, 0));

	return;
}

