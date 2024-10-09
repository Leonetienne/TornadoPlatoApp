#include "Test__MC.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "../Plato/Color.h"
#include "../Plato/Keyboard.h"

using namespace Plato;

/*
	Expected behaviour: See minecraft house in dark. Ceiling light inside should flicker randomly. Press F to toggle flashlight.
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
    mc->transform->Move(Vector3d(0, 5, -15));

    // Create player light
    WorldObject* cameraWo = WorldObjectManager::FindObjectById("main_camera");
    flashlight =
        WorldObjectManager::NewWorldObject("player-light", cameraWo->transform)
        ->AddComponent<Components::PointLight>(3, Color(255, 183, 140));

    // Create a light positioned at the ceiling light
    WorldObject* celingLight = WorldObjectManager::NewWorldObject("ceiling-light");
    celingLight->SetId("ceiling-light");
    celingLight->AddComponent<Components::PointLight>(3, Color(255, 220, 180));
    celingLight->transform->SetPosition(Vector3d(0, 1.3, -9.91571 - 5));

    return;
}

void Test__MC::Update(double deltaTime)
{
    if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::F)) {
        ToggleFlashlight();
    }

    if (rand() % 10 == 0) {
        Components::PointLight* ceilingLight =
            WorldObjectManager::FindObjectById("ceiling-light")->GetComponentOfType<Components::PointLight>();

        // Toggle light
        if (ceilingLight->GetIsEnabled()) {
            ceilingLight->Disable();
        }
        else {
            ceilingLight->Enable();
        }
    }
}

void Test__MC::ToggleFlashlight()
{
    if (flashlight->GetIsEnabled()) {
        TurnFlashlightOff();
    }
    else {
        TurnFlashlightOn();
    }
}

void Test__MC::TurnFlashlightOn()
{
    flashlight->Enable();
}

void Test__MC::TurnFlashlightOff()
{
    flashlight->Disable();
}

void Test__MC::Render(Renderer* renderer)
{
}

