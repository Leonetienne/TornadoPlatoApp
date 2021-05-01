#pragma once
#include "Test__OBJParser__LoadMultimesh.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "Rotator.h"
#include <iostream>

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
	No materials / textures supplied
*/

Test__OBJParser__LoadMultimesh::Test__OBJParser__LoadMultimesh() :
	TestFixture(__FUNCTION__) // Set the test fixtures name e
{
	// Load mesh files
	ResourceManager::LoadMeshFromObj("monke", "monke.obj");
	ResourceManager::LoadMeshFromObj("skybox", "skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("monke", "monke.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "example_skybox.bmp");

	// Create materials
	ResourceManager::NewMaterial("monke")->texture = ResourceManager::FindTexture("monke");
	ResourceManager::NewMaterial("skybox")->texture = ResourceManager::FindTexture("skybox");

	// Create monkey
	WorldObject* monke = WorldObjectManager::NewWorldObject("monke");
	monke->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("monke"),
		ResourceManager::FindMaterial("monke")
	);
	monke->transform->Move(Vector3d::backward * 5);

	// Create skybox
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
		);
	skybox->transform->Scale(Vector3d::one * 50);

	return;
}
