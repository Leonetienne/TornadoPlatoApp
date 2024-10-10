#include "Test__Benchmarkscene.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/PointLight.h"
#include "../Plato/Color.h"
#include "../Plato/Keyboard.h"
#include <iostream>
#include <sstream>

using namespace Plato;

/*
*/
namespace {
}

Test__Benchmarkscene::Test__Benchmarkscene() : TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Load mesh files
	ResourceManager::LoadMeshFromObj("cave", "assets/benchmark-scene/cave.obj");
	ResourceManager::LoadMeshFromObj("cones", "assets/benchmark-scene/cones.obj");
	ResourceManager::LoadMeshFromObj("lamps", "assets/benchmark-scene/lamps.obj");
	ResourceManager::LoadMeshFromObj("lamps-cable", "assets/benchmark-scene/lamp-cable.obj");
	Mesh* lampPoints = ResourceManager::LoadMeshFromObj("lamp-points", "assets/benchmark-scene/lamp-points.obj");
	ResourceManager::LoadMeshFromObj("plants", "assets/benchmark-scene/plants.obj");
	ResourceManager::LoadMeshFromObj("water", "assets/benchmark-scene/water.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("cave", "assets/benchmark-scene/cave-color.png.bmp");
	ResourceManager::LoadTextureFromBmp("cones", "assets/benchmark-scene/cones-color.png.bmp");
	ResourceManager::LoadTextureFromBmp("lamps", "assets/benchmark-scene/lamp-color.png.bmp");
	ResourceManager::LoadTextureFromBmp("lamps-cable", "assets/benchmark-scene/cable-texture.png.bmp");
	ResourceManager::LoadTextureFromBmp("plants", "assets/benchmark-scene/vine-texture.png.bmp");
	ResourceManager::LoadTextureFromBmp("water", "assets/benchmark-scene/water-color.png.bmp");

	// Create materials
	ResourceManager::NewMaterial("cave")->texture = ResourceManager::FindTexture("cave");
	ResourceManager::NewMaterial("cones")->texture = ResourceManager::FindTexture("cones");
	ResourceManager::NewMaterial("lamps-cable")->texture = ResourceManager::FindTexture("lamps-cable");
	ResourceManager::NewMaterial("plants")->texture = ResourceManager::FindTexture("plants");
	ResourceManager::NewMaterial("water")->texture = ResourceManager::FindTexture("water");
	Material* matLamps = ResourceManager::NewMaterial("lamps");
	matLamps->texture = ResourceManager::FindTexture("lamps");
	matLamps->noShading = true;

	// Create world objects
    WorldObject* woCave = WorldObjectManager::NewWorldObject("cave")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("cave"),
            ResourceManager::FindMaterial("cave")
    )->worldObject;

    WorldObject* woCones = WorldObjectManager::NewWorldObject("cones")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("cones"),
            ResourceManager::FindMaterial("cones")
    )->worldObject;

    WorldObject* woLamps = WorldObjectManager::NewWorldObject("lamps")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("lamps"),
            ResourceManager::FindMaterial("lamps")
    )->worldObject;

    WorldObject* woLampsCable = WorldObjectManager::NewWorldObject("lamps-cable")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("lamps-cable"),
            ResourceManager::FindMaterial("lamps-cable")
    )->worldObject;

    WorldObject* woPlants = WorldObjectManager::NewWorldObject("plants")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("plants"),
            ResourceManager::FindMaterial("plants")
    )->worldObject;

    WorldObject* woWater = WorldObjectManager::NewWorldObject("water")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("water"),
            ResourceManager::FindMaterial("water")
    )->worldObject;

    // Create and place lamp poinglights
    std::stringstream ss;
    int i = 0;
    for (const Vector3d& v : lampPoints->v_vertices) {
        ss.str("Hanging Light - ");
        ss << i++;
        WorldObjectManager::NewWorldObject(ss.str())
            ->AddComponent<Components::PointLight>(7, Color(100, 65, 31))
            ->transform->SetPosition(v);
    }

    return;
}

void Test__Benchmarkscene::Update(double deltaTime)
{

}

void Test__Benchmarkscene::Render(Renderer* renderer)
{

}
