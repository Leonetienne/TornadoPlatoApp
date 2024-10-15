#pragma once
#include "Test__FPS.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"

using namespace Plato;

// temporary code
inline double Clamp(double v, double min, double max)
{
	return std::min<double>(std::max<double>(v, min), max);
}

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
    Also a gun and and a skybox should load.
	No materials / textures supplied
    Pressing NUM_3 should aim your gun.
*/

Test__FPS::Test__FPS() :
	Scene(__FUNCTION__) // Set the test fixtures name
{
	// Configure camera
	trCamera = WorldObjectManager::FindObjectById("main_camera")->transform;
	camera = trCamera->worldObject->GetComponentOfType<Components::Camera>();

	// Load mesh files
	ResourceManager::LoadMeshFromObj("gun", "../gun.obj");
	ResourceManager::LoadMeshFromObj("monke", "../monke.obj");
	ResourceManager::LoadMeshFromObj("skybox", "../skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("gun", "../gun.bmp");
	ResourceManager::LoadTextureFromBmp("monke", "../monke.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "../example_skybox.bmp");


	// Create materials
	ResourceManager::NewMaterial("gun")->texture = ResourceManager::FindTexture("gun");
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

	// Create gun holder
	gunHolder = WorldObjectManager::NewWorldObject(
		"gun holder",
		WorldObjectManager::FindObjectById("main_camera")->transform
	);
	gunHolder->transform->SetRotation(Quaternion(Vector3d(0, 90, 0)));
	
	// Create gun
	gun = WorldObjectManager::NewWorldObject("gun", gunHolder->transform);
	gun->transform->SetScale(Vector3d::one * 0.5);

	gun->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("gun"),
		ResourceManager::FindMaterial("gun")
	);

	// Create skybox
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
		);
	skybox->transform->Scale(Vector3d::one * 50);

	// Set gun points
	gunHolderPos_hip = Vector3d(-4, -0.5, -2);
	gunHolderRot_hip = Quaternion(Vector3d(-5, 0, 0));

	gunHolderPos_aim = Vector3d(-4, -0.75, 0);
	gunHolderRot_aim = Quaternion(Vector3d(0, 0, 0));

	// Set gun to hip
	SetGunHip();

	return;
}

void Test__FPS::Update(double deltaTime)
{
	this->deltaTime = deltaTime;
	// Handle user input
	if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::NUM_3))
	{
		isAimed = !isAimed;

		if (isAimed)
			SetGunAimed();
		else
			SetGunHip();
	}

	constexpr double lerpPosSpeed = 0.01;
	constexpr double lerpRotSpeed = 0.008;
	constexpr double lerpFovSpeed = 0.01;

	// Lerp gun position
	gun->transform->SetPosition(
		gun->transform->GetPosition().Lerp(
			targetPos,
			Clamp(lerpPosSpeed * deltaTime, 0, 1)
		)
	);

	// Lerp gun rotation
	gun->transform->SetRotation(
		gun->transform->GetRotation().Lerp(
			targetRot,
			Clamp(lerpRotSpeed * deltaTime, 0, 1)
		)
	);

	// Lerp fov
	// Quick vector hack, too lazy to make new lerp implementation. Gonna put that in a math class
	camera->SetFov(
		Vector2d(camera->GetFov(), 0).Lerp(
			Vector2d(targetFov, 0),
			Clamp(lerpFovSpeed * deltaTime, 0, 1)
		).x
	);

	return;
}

void Test__FPS::Render(Renderer*)
{
	return;
}

void Test__FPS::SetGunAimed()
{
    targetRot = gunHolderRot_aim;
    targetPos = gunHolderPos_aim;
	targetFov = fovAim;

	return;
}

void Test__FPS::SetGunHip()
{
    targetRot = gunHolderRot_hip;
    targetPos = gunHolderPos_hip;
	targetFov = fovHip;

	return;
}
