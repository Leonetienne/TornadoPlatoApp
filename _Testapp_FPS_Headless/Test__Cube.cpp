#include "Test__Cube.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "../Eule/Eule/Vector3.h"
#include "../Plato/Color.h"
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
    Mesh* cube = ResourceManager::NewMesh("cube");
    *cube = Cube();

	// Load texture files
	ResourceManager::LoadTextureFromBmp("furnace", "assets/furnace.bmp");

	// Create materials
	ResourceManager::NewMaterial("furnace")->texture = ResourceManager::FindTexture("furnace");

	// Create Cube
    WorldObject* cubeWo = WorldObjectManager::NewWorldObject("Cube")
        ->AddComponent<Components::MeshRenderer>(
            cube,
            ResourceManager::FindMaterial("furnace")
    )->worldObject;

    // Tilt camera down
    WorldObjectManager::FindObjectById("main_camera")->transform->Rotate(Vector3d(30, 0, 0));

    // Create lights
    Components::PointLight* furnaceLight =
        WorldObjectManager::NewWorldObject("furnace-light", cubeWo->transform)
        ->AddComponent<Components::PointLight>(50, Color(255, 143, 0));
    furnaceLight->transform->Move(Vector3d(0, 3, -2) * 2);

    // Move cube
    cubeWo->transform->Move(Vector3d(0, -2.5, -5));
    cubeWo->transform->Rotate(Vector3d(0, 150, 0));
    cubeWo->transform->Scale(Vector3d(1,1,1) * 2);

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

