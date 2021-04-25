#include "Test__OBJParser__Load.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include <iostream>

Test__OBJParser__Load::Test__OBJParser__Load() :
	TestFixture(__FUNCTION__) // Set the test fixtures name e
{
	ResourceManager::LoadMeshFromObj("cube", "cube.obj");
	ResourceManager::LoadMeshFromObj("bottle", "bottle.obj");
	ResourceManager::LoadMeshFromObj("spoder", "spoder__gitignore_.obj");

	ResourceManager::LoadTextureFromBmp("bottle", "bottle text.bmp");
	ResourceManager::LoadTextureFromBmp("spoder", "spoder__gitignore_.bmp");

	ResourceManager::NewMaterial("bottle")->texture = ResourceManager::FindTexture("bottle");
	ResourceManager::NewMaterial("spoder")->texture = ResourceManager::FindTexture("spoder");

	for (int i = 0; i <= 6; i++)
	for (int j = 0; j <= 6; j++)
	{
		wo_mesh = WorldObjectManager::NewWorldObject("spoder");
		wo_mesh->AddComponent<MeshRenderer>(
			ResourceManager::FindMesh("spoder"),
			ResourceManager::FindMaterial("spoder")
		);

		wo_mesh->transform->SetPosition(Vector3d((3-i)*40, 0, -j*40));
		wo_mesh->transform->Move(Vector3d(0, -30, -50));
		wo_mesh->transform->Rotate(Quaternion(Vector3d(0, 90, 0)));
	}


	return;
}
