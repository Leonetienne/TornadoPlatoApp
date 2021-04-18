#include "TransformHierarchy__Curl.h"
#include "Cube.h"
#include "../Plato/Renderer.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/bmplib.h"
#include "Rotator.h"
#include "Util.h"

/*
	This test tests the transform hierarchy resolving rotations.
	It does this by "curling a finger".
	It stacks a lot of cubes onto another and parents them in to each other. Each cubes lower cube is its parent.
	Then it applies a local rotation to all cubes. They should curl in like a finger.

*/

Transform* TransformHierarchy__Curl::CreateLink(Transform* parent)
{
	WorldObject* wo = WorldObjectManager::NewWorldObject("joint", parent);
	wo->AddComponent<MeshRenderer>(&mesh_coob, &mat_coob);
	
	wo->AddTag("joint");
	wo->GetTransform()->Move(Vector3d::up * 3);

	return wo->GetTransform();
}

Transform* jointRoot;

TransformHierarchy__Curl::TransformHierarchy__Curl() :
	TestFixture(__FUNCTION__), // Set the test fixtures name equal to the specialized class name (constructor function)
	txt_coob(Color::black)
{
	// Reposition camera to a more fitting place
	{
		Transform* cam = WorldObjectManager::FindObjectById("main_camera_ypiv")->GetTransform();
		cam->SetPosition(Vector3d(20, 10, 0));
		cam->SetRotation(Quaternion::FromEuler(Vector3d(0, -90, 0)));
	}

	// Load cube mesh and texture
	mesh_coob = Cube();
	LoadTextureFromBmp(&txt_coob, "../Plato/Cube_furnace_gitignore_.bmp");
	mat_coob.texture = &txt_coob;

	// Create a root transform (for positioning and rotation)
	jointRoot = WorldObjectManager::NewWorldObject("joint_root")->GetTransform();
	jointRoot->Rotate(Quaternion::FromEuler(Vector3d::up * 180));

	// Create five joints
	Transform* lastParent = jointRoot;
	for (std::size_t i = 0; i < 15; i++)
		lastParent = CreateLink(lastParent);

	return;
}

void TransformHierarchy__Curl::Update(double frametime)
{
	if (GetAsyncKeyState(VK_SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->GetTransform()->Rotate(Quaternion::FromEuler(Vector3d::right * 0.07));

	if (GetAsyncKeyState('U'))
		jointRoot->Rotate(Quaternion::FromEuler(Vector3d(-2.5, -2, -8) * 0.1));

	return;
}
