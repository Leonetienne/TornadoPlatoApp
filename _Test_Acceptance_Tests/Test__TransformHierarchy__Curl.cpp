#include "Test__TransformHierarchy__Curl.h"
#include "../StaticMeshClasses/Cube.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"

using namespace Plato;

/*
	This test tests the transform hierarchy resolving rotations.
	It does this by "curling a finger".
	It stacks a lot of cubes onto another and parents them in to each other. Each cubes lower cube is its parent.
	Then it applies a local rotation to all cubes. They should curl in like a finger.

	Expected behaviour:
	When pressing SPACE, the stack of objects should curl like a finger. The objects inside-the-curl faces should always face inside the curl.
    The stack should be hardcoded-randomly rotated initially. This is because we dont want a clean rotation to start with. That's too easy.
    Presssing [U] roates the entire stack.
*/

Transform* Test__TransformHierarchy__Curl::CreateLink(Transform* parent)
{
	WorldObject* wo = WorldObjectManager::NewWorldObject("joint", parent);
	wo->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("cube"),
		ResourceManager::FindMaterial("cube")
	);
	
	wo->AddTag("joint");
	wo->transform->Move(Vector3d::up * 3);

	return wo->transform;
}

Test__TransformHierarchy__Curl::Test__TransformHierarchy__Curl() :
	Scene(__FUNCTION__) // Set the test fixtures name equal to the specialized class name (constructor function)
{
	// Reposition camera to a more fitting place
	{
		Transform* cam = WorldObjectManager::FindObjectById("main_camera_ypiv")->transform;
		cam->SetPosition(Vector3d(20, 3, 0));
		cam->SetRotation(Quaternion(Vector3d(0, -90, 0)));
	}

	// Create and load assets
	*ResourceManager::NewMesh("cube") = Cube();
	ResourceManager::LoadTextureFromBmp("cube", "../furnace.bmp");
	ResourceManager::NewMaterial("cube")->texture = ResourceManager::FindTexture("cube");

	// Create a root transform (for positioning and rotation)
	jointRoot = WorldObjectManager::NewWorldObject("joint_root")->transform;
	jointRoot->Rotate(Quaternion(Vector3d(49, -20, 39)));
	jointRoot->Scale(Vector3d::one * 0.8);

	// Create fifteen joints
	Transform* lastParent = jointRoot;
	for (std::size_t i = 0; i < 15; i++)
		lastParent = CreateLink(lastParent);

	return;
}

void Test__TransformHierarchy__Curl::Update(double frametime)
{
	if (Input::Keyboard::GetKey(Input::KEY_CODE::SPACE))
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("joint"))
			wo->transform->Rotate(Quaternion(Vector3d::right * 0.07));

	if (Input::Keyboard::GetKey(Input::KEY_CODE::U))
		jointRoot->Rotate(Quaternion(Vector3d(-2.5, -2, -8) * 0.1));

	return;
}
