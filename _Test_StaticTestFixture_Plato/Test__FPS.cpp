#pragma once
#include "Test__FPS.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"
#include "Rotator.h"
#include <iostream>

// temporary code
inline double Clamp(double v, double min, double max)
{
	return std::min<double>(std::max<double>(v, min), max);
}

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
	No materials / textures supplied
*/

Test__FPS::Test__FPS() :
	TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Configure camera
	trCamera = WorldObjectManager::FindObjectById("main_camera")->transform;
	camera = trCamera->worldObject->GetComponentOfType<Camera>();

	// Load mesh files
	ResourceManager::LoadMeshFromObj("gun", "gun.obj");
	ResourceManager::LoadMeshFromObj("monke", "monke.obj");
	ResourceManager::LoadMeshFromObj("skybox", "skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("gun", "gun.bmp");
	ResourceManager::LoadTextureFromBmp("monke", "monke.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "example_skybox.bmp");


	// Create materials
	ResourceManager::NewMaterial("gun")->texture = ResourceManager::FindTexture("gun");
	ResourceManager::NewMaterial("monke")->texture = ResourceManager::FindTexture("monke");
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = true;

	// Create monkey
	WorldObject* monke = WorldObjectManager::NewWorldObject("monke");
	monke->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("monke"),
		ResourceManager::FindMaterial("monke")
		);
	monke->transform->Move(Vector3d::backward * 5);

	// Create gun holder
	gunHolder = WorldObjectManager::NewWorldObject(
		"gun holder",
		WorldObjectManager::FindObjectById("main_camera")->transform
	);
	gunHolder->transform->SetRotation(Quaternion(Vector3d(-5, 90, 0)));
	gunHolder->transform->SetPosition(Vector3d(2, -1, -4));
	
	// Create gun
	gun = WorldObjectManager::NewWorldObject("gun");
	gun->transform->SetScale(Vector3d::one * 0.5);

	gun->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("gun"),
		ResourceManager::FindMaterial("gun")
	);

	// Create skybox
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
		);
	skybox->transform->Scale(Vector3d::one * 50);

	// Set gun points
	gunHolderPos_hip = Vector3d(2, -1, -4) * 0.75;
	gunHolderRot_hip = Quaternion(Vector3d(-5, 90, 0));

	gunHolderPos_aim = Vector3d(0, -1.0, -4) * 0.75;
	gunHolderRot_aim = Quaternion(Vector3d(0, 90, 0));

	// Set gun to hip
	SetGunHip();

	return;
}

void Test__FPS::Update(double deltaTime)
{
	constexpr double lerpSpeed = 0.025;

	// Lerp gun position
	gun->transform->SetPosition(
		gun->transform->GetGlobalPosition().Lerp(
			gunHolder->transform->GetGlobalPosition(),
			Clamp(lerpSpeed * deltaTime, 0, 1)
		)
	);

	// Lerp gun rotation
	gun->transform->SetRotation(
		gun->transform->GetGlobalRotation().Lerp(
			gunHolder->transform->GetGlobalRotation(),
			Clamp(lerpSpeed * deltaTime, 0, 1)
		)
	);

	// Lerp fov
	// Quick vector hack, too lazy to make new lerp implementation. Gonna put that in a math class
	camera->SetFov(
		Vector2d(camera->GetFov(), 0).Lerp(
			Vector2d(targetFov, 0),
			Clamp(lerpSpeed * deltaTime, 0, 1)
		).x
	);

	// Handle user input
	if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::MOUSE_R))
	{
		isAimed = !isAimed;

		if (isAimed)
			SetGunAimed();
		else
			SetGunHip();
	}

	return;
}

void Test__FPS::SetGunAimed()
{
	gunHolder->transform->SetPosition(gunHolderPos_aim);
	gunHolder->transform->SetRotation(gunHolderRot_aim);
	targetFov = fovAim;

	return;
}

void Test__FPS::SetGunHip()
{
	gunHolder->transform->SetPosition(gunHolderPos_hip);
	gunHolder->transform->SetRotation(gunHolderRot_hip);
	targetFov = fovHip;

	return;
}
