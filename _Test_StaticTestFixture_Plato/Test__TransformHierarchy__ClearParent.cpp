#include "Test__TransformHierarchy__ClearParent.h"
#include "Cube.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/bmplib.h"
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
	wo->AddComponent<MeshRenderer>(&mesh_coob, &mat_coob);

	wo->AddTag("joint");
	wo->GetTransform()->Move(Vector3d::up * 3);
	wo->GetTransform()->Rotate(Vector3d::right * 20);

	return wo->GetTransform();
}

Test__TransformHierarchy__ClearParent::Test__TransformHierarchy__ClearParent() :
	TestFixture(__FUNCTION__), // Set the test fixtures name equal to the specialized class name (constructor function)
	txt_coob(Color::black)
{
	// Reposition camera to a more fitting place
	{
		Transform* cam = WorldObjectManager::FindObjectById("main_camera_ypiv")->GetTransform();
		cam->SetPosition(Vector3d(20, -3, 0));
		cam->SetRotation(Quaternion::FromEuler(Vector3d(0, -90, 0)));
	}

	// Load cube mesh and texture
	mesh_coob = Cube();
	LoadTextureFromBmp(&txt_coob, "../Plato/Cube_furnace_gitignore_.bmp");
	mat_coob.texture = &txt_coob;

	// Create a root transform (for positioning and rotation)
	jointRoot = WorldObjectManager::NewWorldObject("joint_root")->GetTransform();
	jointRoot->Rotate(Quaternion::FromEuler(Vector3d(60, -20, 15)));
	jointRoot->Scale(Vector3d::one * 1.1);

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
			wo->GetTransform()->SetParent(nullptr);

	// Some fun-control. Will rotate all objects (locally)
	if (GetAsyncKeyState(VK_SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->GetTransform()->Rotate(Quaternion::FromEuler(Vector3d::right * 0.03));

	return;
}

