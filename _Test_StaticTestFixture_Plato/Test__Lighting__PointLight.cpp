#pragma once
#include "Test__Lighting__PointLight.h"
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
	Expected behaviour: 
*/

Test__Lighting__PointLight::Test__Lighting__PointLight() :
	TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Configure camera
	trCamera = WorldObjectManager::FindObjectById("main_camera")->transform;
	camera = trCamera->worldObject->GetComponentOfType<Camera>();

	// Create plane mesh
	Mesh* mPlane = ResourceManager::NewMesh("plane");
	mPlane->v_vertices = {
		Vector3d(-1, 0, -1),
		Vector3d(1, 0, -1),
		Vector3d(1, 0, 1),
		Vector3d(-1, 0, 1)
	};
	mPlane->uv_vertices = {
		Vector2d(0, 0),
		Vector2d(1, 0),
		Vector2d(1, 1),
		Vector2d(0, 1)
	};
	mPlane->normals = {
		Vector3d::up
	};
	mPlane->tris = {
		{0, 0, 0},
		{2, 2, 0},
		{1, 1, 0},
		{3, 3, 0},
		{2, 2, 0},
		{0, 0, 0},
	};

	// Load mesh files
	ResourceManager::LoadMeshFromObj("gun", "gun.obj");
	ResourceManager::LoadMeshFromObj("monke", "monke.obj");
	ResourceManager::LoadMeshFromObj("skybox", "skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("gun", "gun.bmp");
	ResourceManager::LoadTextureFromBmp("monke", "monke.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "example_skybox.bmp");
	ResourceManager::LoadTextureFromBmp("floor", "texture_floor__gitignore_.bmp");


	// Create materials
	ResourceManager::NewMaterial("gun")->texture = ResourceManager::FindTexture("gun");
	ResourceManager::NewMaterial("monke")->texture = ResourceManager::FindTexture("monke");
	ResourceManager::NewMaterial("floor")->texture = ResourceManager::FindTexture("floor");
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = true;

	// Create plane
	WorldObject* woPlane = WorldObjectManager::NewWorldObject("plane");
	woPlane->AddComponent<MeshRenderer>(mPlane, ResourceManager::FindMaterial("floor"));
	woPlane->transform->Scale(Vector3d::one * 40);
	woPlane->transform->Move(Vector3d::down * 2 + Vector3d::backward * 10);

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
	gun->Disable();

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
	skybox->Disable();

	// Set gun points
	gunHolderPos_hip = Vector3d(2, -1, -4) * 0.75;
	gunHolderRot_hip = Quaternion(Vector3d(-5, 90, 0));

	gunHolderPos_aim = Vector3d(0, -1.0, -4) * 0.75;
	gunHolderRot_aim = Quaternion(Vector3d(0, 90, 0));

	// Set gun to hip
	SetGunHip();


	// Create point light
	WorldObject* woLight = WorldObjectManager::NewWorldObject("point light");
	pointLight = woLight->AddComponent<PointLight>(100, Color(0, 130, 255));
	pointLight->transform->Move(Vector3d::left * 4 + Vector3d::backward * 3);

	woLight = WorldObjectManager::NewWorldObject("point light");
	pointLight = woLight->AddComponent<PointLight>(100, Color(255, 130, 0));
	pointLight->transform->Move(Vector3d::right * 4 + Vector3d::backward * 3);

	return;
}

void Test__Lighting__PointLight::Update(double deltaTime)
{
	this->deltaTime = deltaTime;
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

void Test__Lighting__PointLight::Render(Renderer*)
{
	constexpr double lerpPosSpeed = 0.07;
	constexpr double lerpRotSpeed = 0.04;
	constexpr double lerpFovSpeed = 0.005;

	// Lerp gun position
	gun->transform->SetPosition(
		gun->transform->GetGlobalPosition().Lerp(
			gunHolder->transform->GetGlobalPosition(),
			Clamp(lerpPosSpeed * deltaTime, 0, 1)
		)
	);

	// Lerp gun rotation
	gun->transform->SetRotation(
		gun->transform->GetGlobalRotation().Lerp(
			gunHolder->transform->GetGlobalRotation(),
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

void Test__Lighting__PointLight::SetGunAimed()
{
	gunHolder->transform->SetPosition(gunHolderPos_aim);
	gunHolder->transform->SetRotation(gunHolderRot_aim);
	targetFov = fovAim;

	return;
}

void Test__Lighting__PointLight::SetGunHip()
{
	gunHolder->transform->SetPosition(gunHolderPos_hip);
	gunHolder->transform->SetRotation(gunHolderRot_hip);
	targetFov = fovHip;

	return;
}
