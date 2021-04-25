#include "Test__OBJParser__Load.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include <iostream>

Test__OBJParser__Load::Test__OBJParser__Load() :
	TestFixture(__FUNCTION__) // Set the test fixtures name e
{
	ResourceManager::LoadMeshFromObj("cube", "cube.obj");
	ResourceManager::LoadMeshFromObj("bottle", "bottle.obj");

	ResourceManager::LoadTextureFromBmp("bottle", "bottle text.bmp");
	ResourceManager::NewMaterial("bottle")->texture = ResourceManager::FindTexture("bottle");

	wo_mesh = WorldObjectManager::NewWorldObject("bottle");
	wo_mesh->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("bottle"),
		ResourceManager::FindMaterial("bottle")
	);

	return;
}
