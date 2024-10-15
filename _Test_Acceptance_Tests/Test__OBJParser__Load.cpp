#include "Test__OBJParser__Load.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"

using namespace Plato;

/*
	Expected behaviour: The objects load
*/

Test__OBJParser__Load::Test__OBJParser__Load() :
	Scene(__FUNCTION__) // Set the test fixtures name
{
	// Load mesh files
	ResourceManager::LoadMeshFromObj("cube", "cube.obj");
	ResourceManager::LoadMeshFromObj("bottle", "bottle.obj");
	ResourceManager::LoadMeshFromObj("spoder", "spoder__gitignore_.obj");
	ResourceManager::LoadMeshFromObj("character", "character__gitignore_.obj");
	ResourceManager::LoadMeshFromObj("skybox", "skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("bottle", "bottle text.bmp");
	ResourceManager::LoadTextureFromBmp("spoder", "spoder__gitignore_.bmp");
	ResourceManager::LoadTextureFromBmp("character", "character__gitignore_.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "example_skybox.bmp");

	// Create materials
	ResourceManager::NewMaterial("bottle")->texture = ResourceManager::FindTexture("bottle");
	ResourceManager::NewMaterial("spoder")->texture = ResourceManager::FindTexture("spoder");
	ResourceManager::NewMaterial("character")->texture = ResourceManager::FindTexture("character");
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = true;

	// Create spider
	WorldObject* spider = WorldObjectManager::NewWorldObject("spoder");
	spider->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("spoder"),
		ResourceManager::FindMaterial("spoder")
	);
	spider->transform->Move(Vector3d(0, -15, -50));
	spider->transform->Rotate(Quaternion(Vector3d(0, 90, 0)));

	WorldObject* character = WorldObjectManager::NewWorldObject("character", spider->transform);
	character->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("character"),
		ResourceManager::FindMaterial("character")
		);

	// Create skybox
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
	);
	skybox->transform->Scale(Vector3d::one * 50);

	return;
}
