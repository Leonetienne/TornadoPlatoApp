#include "Test__OBJParser__Load.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "Rotator.h"
#include <iostream>

Test__OBJParser__Load::Test__OBJParser__Load() :
	TestFixture(__FUNCTION__) // Set the test fixtures name e
{
	ResourceManager::LoadMeshFromObj("cube", "cube.obj");
	ResourceManager::LoadMeshFromObj("bottle", "bottle.obj");
	ResourceManager::LoadMeshFromObj("spoder", "spoder__gitignore_.obj");
	ResourceManager::LoadMeshFromObj("character", "character__gitignore_.obj");
	ResourceManager::LoadMeshFromObj("skybox", "skybox.obj");

	ResourceManager::LoadTextureFromBmp("bottle", "bottle text.bmp");
	ResourceManager::LoadTextureFromBmp("spoder", "spoder__gitignore_.bmp");
	ResourceManager::LoadTextureFromBmp("character", "character__gitignore_.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "example_skybox.bmp");

	ResourceManager::NewMaterial("bottle")->texture = ResourceManager::FindTexture("bottle");
	ResourceManager::NewMaterial("spoder")->texture = ResourceManager::FindTexture("spoder");
	ResourceManager::NewMaterial("character")->texture = ResourceManager::FindTexture("character");
	ResourceManager::NewMaterial("skybox")->texture = ResourceManager::FindTexture("skybox");

	// Create spider
	wo_mesh = WorldObjectManager::NewWorldObject("spoder");
	wo_mesh->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("spoder"),
		ResourceManager::FindMaterial("spoder")
	);
	wo_mesh->transform->Move(Vector3d(0, -15, -50));
	wo_mesh->transform->Rotate(Quaternion(Vector3d(0, 90, 0)));

	wo_mesh = WorldObjectManager::NewWorldObject("character", wo_mesh->transform);
	wo_mesh->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("character"),
		ResourceManager::FindMaterial("character")
		);

	// Create skybox
	Transform* skybox = WorldObjectManager::NewWorldObject("skybox")->transform;
	skybox->worldObject->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
	);
	skybox->Scale(Vector3d::one * 50);

	return;
}
