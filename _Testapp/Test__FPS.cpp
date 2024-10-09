#include "Test__FPS.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"
#include "../Plato/PointLight.h"
#include "../Plato/Color.h"
#include <iostream>

using namespace Plato;

// temporary code
namespace {
    inline double Clamp(double v, double min, double max) {
        return std::min<double>(std::max<double>(v, min), max);
    }
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
	camera = trCamera->worldObject->GetComponentOfType<Components::Camera>();

	// Load mesh files
	ResourceManager::LoadMeshFromObj("gun", "assets/gun.obj");
	ResourceManager::LoadMeshFromObj("monke", "assets/monke.obj");
	ResourceManager::LoadMeshFromObj("skybox", "assets/skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("gun", "assets/gun.bmp");
	ResourceManager::LoadTextureFromBmp("monke", "assets/monke.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "assets/example_skybox.bmp");


	// Create materials
	ResourceManager::NewMaterial("gun")->texture = ResourceManager::FindTexture("gun");
	ResourceManager::NewMaterial("monke")->texture = ResourceManager::FindTexture("monke");
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = false;

	// Create monkey
	WorldObject* monke = WorldObjectManager::NewWorldObject("monke");
	monke->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("monke"),
		ResourceManager::FindMaterial("monke")
    );
	monke->transform->Move(Vector3d::backward * 5);
    monke->transform->Rotate(Vector3d::up * 30);

	// Create gun holder
	gunHolder = WorldObjectManager::NewWorldObject(
		"gun holder",
		WorldObjectManager::FindObjectById("main_camera")->transform
	);
	gunHolder->transform->SetRotation(Quaternion(Vector3d(-5, 90, 0)));
	gunHolder->transform->SetPosition(Vector3d(2, -1, -4));
	
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
	gunHolderPos_hip = Vector3d(2, -1, -4) * 0.75;
	gunHolderRot_hip = Quaternion(Vector3d(-5, 90, 0));

	gunHolderPos_aim = Vector3d(0, -1.0, -4) * 0.75;
	gunHolderRot_aim = Quaternion(Vector3d(0, 90, 0));

    Components::PointLight* leftLight = WorldObjectManager::NewWorldObject("left-light")->AddComponent<Components::PointLight>(150, Color(255, 200, 150));
    Components::PointLight* rightLight = WorldObjectManager::NewWorldObject("right-light")->AddComponent<Components::PointLight>(150, Color(150, 200, 255));
    leftLight->transform->Move(Vector3d(-1, 1, 1) * 5);
    rightLight->transform->Move(Vector3d(1, -1, 1) * 5);

	// Set gun to hip
	SetGunHip();

	return;
}

void Test__FPS::Update(double deltaTime)
{
	return;
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

void Test__FPS::Render(Renderer* renderer)
{
	constexpr double lerpPosSpeed = 0.07;
	constexpr double lerpRotSpeed = 0.04;
	constexpr double lerpFovSpeed = 0.05;

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
