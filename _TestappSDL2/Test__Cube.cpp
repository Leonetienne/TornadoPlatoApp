#include "Test__Cube.h"
#include "../HandyComponents/Rotator.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "../Plato/Vector.h"
#include "../Plato/Color.h"
#include "../Plato/Keyboard.h"
#include "Cube.h"

using namespace Plato;

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
	No materials / textures supplied
*/

namespace {
}

Test__Cube::Test__Cube() : TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Configure camera
    Mesh* cubeMesh = ResourceManager::NewMesh("cube");
    *cubeMesh = Cube();

	// Load texture files
	ResourceManager::LoadTextureFromBmp("furnace", "assets/furnace.bmp");

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
    WorldObjectManager::FindObjectById("main_camera")->transform->Rotate(Vector3d(30, 0, 0));

    // Create lights
    Components::PointLight* furnaceLight =
        WorldObjectManager::NewWorldObject("furnace-light")
        ->AddComponent<Components::PointLight>(20, Color(255, 230, 220));
    furnaceLight->transform->Move(Vector3d(-2, 3*5, -2));
    light = furnaceLight->worldObject;

    // Move cube
    cube->transform->Move(Vector3d(0, -2.5, -5));
    cube->transform->Rotate(Vector3d(0, 0, 0));
    cube->transform->Scale(Vector3d::one * 5);

    return;
}

void Test__Cube::Update(double deltaTime)
{
	return;
}

void Test__Cube::Render(Renderer* renderer)
{
	return;
}

