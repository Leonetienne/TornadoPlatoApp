#include "MC_FurnaceScene.h"
#include "../../../StaticMeshClasses/Cube.h"
#include "../../../HandyComponents/Rotator.h"
#include "../../../Plato/WorldObjectManager.h"
#include "../../../Plato/ResourceManager.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/Material.h"
#include "../../../Plato/Color.h"
#include <stdexcept>

using namespace Plato;

/*
	Expected behaviour: See minecraft house in dark. Ceiling light inside should flicker randomly. Press F to toggle flashlight.
*/
namespace {
}

MC_FurnaceScene::MC_FurnaceScene() : Scene(__FUNCTION__) // Set the test fixtures name
{
    const std::string assetsDir = "../../Scenes/Fun/MC_Furnace/assets";

	// Configure camera
    Mesh* cubeMesh = ResourceManager::NewMesh("cube");
    *cubeMesh = Cube();

	// Load texture files
	ResourceManager::FindTextureOrLoadFromBmp("furnace", assetsDir+"/furnace.bmp");

	// Create materials
	ResourceManager::NewMaterial("furnace")->texture = ResourceManager::FindTexture("furnace");

	// Create Cube
    cube = WorldObjectManager::NewWorldObject("Cube")
        ->AddComponent<Components::MeshRenderer>(
            cubeMesh,
            ResourceManager::FindMaterial("furnace")
    )->worldObject;
    cube->AddComponent<Rotator>();

    // Tilt camera down
    WorldObject* cameraWo = WorldObjectManager::FindObjectById("main_camera");
    if (!cameraWo) {
        throw std::runtime_error(std::string("Attempted to initialize scene ") + GetSceneName() + " without having a any camera loaded! But that is required!");
    }
    cameraWo->transform->Rotate(Vector3d(30, 0, 0));

    // Create lights
    Components::PointLight* furnaceLight =
        WorldObjectManager::NewWorldObject("furnace-light")
        ->AddComponent<Components::PointLight>(20, Color(255, 230, 220));
    furnaceLight->transform->Move(Vector3d(-2, 2, -2));
    light = furnaceLight->worldObject;

    // Move cube
    this->cube->transform->Move(Vector3d(0, -2.5, -5));
    this->cube->transform->Rotate(Vector3d(0, 180, 0));

    return;
}


