#include "Test__Cube.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "../Eule/Eule/Vector3.h"
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
    this->cube = WorldObjectManager::NewWorldObject("Cube")
        ->AddComponent<Components::MeshRenderer>(
            cubeMesh,
            ResourceManager::FindMaterial("furnace")
    )->worldObject;

    // Tilt camera down
    WorldObjectManager::FindObjectById("main_camera")->transform->Rotate(Vector3d(30, 0, 0));

    // Create lights
    Components::PointLight* furnaceLight =
        WorldObjectManager::NewWorldObject("furnace-light")
        ->AddComponent<Components::PointLight>(20, Color(255, 143, 0));
    furnaceLight->transform->Move(Vector3d(-2, 2, -2));
    light = furnaceLight->worldObject;

    // Move cube
    this->cube->transform->Move(Vector3d(0, -2.5, -5));
    this->cube->transform->Rotate(Vector3d(0, 150, 0));
    this->cube->transform->Scale(Vector3d(1,1,1) * 2);

    return;
}

void Test__Cube::Update(double deltaTime)
{
    if (Input::Keyboard::GetKeyDown((Input::KEY_CODE)'o')) {
        speedFac -= 1.0;
    }
    if (Input::Keyboard::GetKeyDown((Input::KEY_CODE)'p')) {
        speedFac += 1.0;
    }

    if (Input::Keyboard::GetKeyDown((Input::KEY_CODE)'a')) {
        light->transform->Move(Vector3d(-1, 0, 0));
    }
    if (Input::Keyboard::GetKeyDown((Input::KEY_CODE)'d')) {
        light->transform->Move(Vector3d(1, 0, 0));
    }
    if (Input::Keyboard::GetKeyDown((Input::KEY_CODE)'w')) {
        light->transform->Move(Vector3d(0, 0, -1));
    }
    if (Input::Keyboard::GetKeyDown((Input::KEY_CODE)'s')) {
        light->transform->Move(Vector3d(0, 0, 1));
    }

    this->cube->transform->Rotate(Vector3d(
        15.0 * speedFac * deltaTime,
        10.0 * speedFac * deltaTime,
        5.0 * speedFac * deltaTime
    ));
	return;
}

void Test__Cube::Render(Renderer* renderer)
{
	return;
}

