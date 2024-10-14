#include "Test__Cube.h"
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
        ->AddComponent<Components::PointLight>(20, Color(255, 230, 220));
    furnaceLight->transform->Move(Vector3d(-2, 3*5, -2));
    light = furnaceLight->worldObject;

    // Move cube
    this->cube->transform->Move(Vector3d(0, -2.5, -5));
    this->cube->transform->Rotate(Vector3d(0, 0, 0));
    this->cube->transform->Scale(Vector3d::one * 5);

    return;
}

void Test__Cube::Update(double deltaTime)
{
    static Vector3d speedFac = Vector3d::zero;

    if (Input::Keyboard::GetKey(Input::KEY_CODE::I)) {
        speedFac.x += 0.005 * deltaTime;
    }
    if (Input::Keyboard::GetKey(Input::KEY_CODE::K)) {
        speedFac.x -=  0.005 * deltaTime;
    }
    if (Input::Keyboard::GetKey(Input::KEY_CODE::L)) {
        speedFac.z += 0.005 * deltaTime;
    }
    if (Input::Keyboard::GetKey(Input::KEY_CODE::J)) {
        speedFac.z -=  0.005 * deltaTime;
    }
    if (Input::Keyboard::GetKey(Input::KEY_CODE::U)) {
        speedFac.y += 0.005 * deltaTime;
    }
    if (Input::Keyboard::GetKey(Input::KEY_CODE::O)) {
        speedFac.y -=  0.005 * deltaTime;
    }

    if (Input::Keyboard::GetKey(Input::KEY_CODE::SPACE)) {
        speedFac = Vector3d::zero;
    }

    this->cube->transform->Rotate(Vector3d(
        0.025 * speedFac.x * deltaTime,
        0.025 * speedFac.y * deltaTime,
        0.025 * speedFac.z * deltaTime
    ));
	return;
}

void Test__Cube::Render(Renderer* renderer)
{
	return;
}

