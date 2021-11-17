#pragma once
#include "Test__OBJParser__LoadMultimesh.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "Rotator.h"
#include <iostream>

using namespace Plato;

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
	No materials / textures supplied
*/

Test__OBJParser__LoadMultimesh::Test__OBJParser__LoadMultimesh() :
	TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Load mesh files
	ResourceManager::LoadMeshFromObj("monke", "monke.obj");
	ResourceManager::LoadMeshFromObj("skybox", "skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("monke", "monke.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "example_skybox.bmp");


	// Create materials
	ResourceManager::NewMaterial("monke")->texture = ResourceManager::FindTexture("monke");
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = true;

	// Create monkey
	WorldObject* monke = WorldObjectManager::NewWorldObject("monke");
	monke->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("monke"),
		ResourceManager::FindMaterial("monke")
	);
	monke->transform->Move(Vector3d::backward * 5);

	//monke->AddComponent<Rotator>(Vector3d(0.5, 1, 1.5));

	// Create skybox
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
		);
	skybox->transform->Scale(Vector3d::one * 50);

	return;
}
