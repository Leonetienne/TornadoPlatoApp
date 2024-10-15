#include "MC_HouseScene.h"
#include "../../../Prefabs/Skybox/SkyboxPrefab.h"
#include "../../../Plato/WorldObjectManager.h"
#include "../../../Plato/ResourceManager.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/Material.h"
#include "../../../Plato/Color.h"
#include "../../../Plato/Keyboard.h"
#include <stdexcept>

using namespace Plato;

/*
	Expected behaviour: See minecraft house in dark. Ceiling light inside should flicker randomly. Press F to toggle flashlight.
*/
namespace {
}

MC_HouseScene::MC_HouseScene() : Scene(__FUNCTION__) // Set the test fixtures name
{
    const std::string assetsDir = "../../Scenes/Fun/MC_House/assets";

	// Load texture files
    Texture* texture = ResourceManager::FindTextureOrLoadFromBmp("mc_world", assetsDir+"/mc_world_tran.bmp");

	// Load mesh files
    Mesh* mesh = ResourceManager::FindMeshOrLoadFromObj("mc_world", assetsDir+"/mc_world.obj");

	// Create materials
    Material* mat = ResourceManager::NewMaterial("mc_world");
    mat->texture = texture;

	// Create the mc world object
    WorldObject* mc = WorldObjectManager::NewWorldObject("mc_world")
        ->AddComponent<Components::MeshRenderer>(
            mesh,
            mat
    )->worldObject;

    // Create a skybox
    (SkyboxPrefab()).Instantiate();

    // Move it, to position the player infront of it
    mc->transform->Rotate(Quaternion(Vector3d(0, 90, 0)));
    mc->transform->Move(Vector3d(0, 5, -15));

    // Create player light
    WorldObject* cameraWo = WorldObjectManager::FindObjectById("main_camera");
    if (!cameraWo) {
        throw std::runtime_error(std::string("Attempted to initialize scene ") + GetSceneName() + " without having a any camera loaded! But that is required!");
    }
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

void MC_HouseScene::Update(double deltaTime)
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

void MC_HouseScene::ToggleFlashlight()
{
    if (flashlight->GetIsEnabled()) {
        TurnFlashlightOff();
    }
    else {
        TurnFlashlightOn();
    }
}

void MC_HouseScene::TurnFlashlightOn()
{
    flashlight->Enable();
}

void MC_HouseScene::TurnFlashlightOff()
{
    flashlight->Disable();
}

