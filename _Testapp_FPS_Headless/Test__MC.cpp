#include "Test__MC.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "../Plato/Color.h"
#include "../Plato/Keyboard.h"
#include <iostream>

using namespace Plato;

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
	No materials / textures supplied
*/

namespace {
}

Test__MC::Test__MC() : TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Load mesh files
	ResourceManager::LoadMeshFromObj("mc_world", "assets/mc_world.obj");
	ResourceManager::LoadMeshFromObj("skybox", "assets/skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("mc_world", "assets/mc_world_tran.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "assets/example_skybox.bmp");

	// Create materials
	ResourceManager::NewMaterial("mc_world")->texture = ResourceManager::FindTexture("mc_world");
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = false;

	// Create the skybox worlds object
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
		);
	skybox->transform->Scale(Vector3d::one * 50);

	// Create the mc world object
    WorldObject* mc = WorldObjectManager::NewWorldObject("mc_world")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("mc_world"),
            ResourceManager::FindMaterial("mc_world")
    )->worldObject;

    // Move it, to position the player infront of it
    mc->transform->Rotate(Quaternion(Vector3d(0, 90, 0)));
    mc->transform->Move(Vector3d(0, 5, -10));

    // Create player light
    WorldObject* cameraWo = WorldObjectManager::FindObjectById("main_camera");
    Components::PointLight* playerLight =
        WorldObjectManager::NewWorldObject("player-light", cameraWo->transform)
        ->AddComponent<Components::PointLight>(0, Color(255, 183, 140));

    // Create a light positioned at the ceiling light
    WorldObjectManager::NewWorldObject("ceiling-light")
        ->AddComponent<Components::PointLight>(15, Color(255, 183, 140))
        ->transform->SetPosition(Vector3d(0, 1.3, -9.91571));

    return;
}

void Test__MC::Update(double deltaTime)
{
    if (Input::Keyboard::GetKey(Input::KEY_CODE::Z)) {
        std::cout << WorldObjectManager::FindObjectById("main_camera")->transform->GetGlobalPosition() << std::endl;
        exit(0);
    }
}

void Test__MC::Render(Renderer* renderer)
{
	return;
}

