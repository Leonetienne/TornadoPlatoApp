#include "Dust2Scene.h"
#include "../../../Prefabs/Skybox/SkyboxPrefab.h"
#include "../../../Plato/MTLParser.h"
#include "../../../Plato/MeshRenderer.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/WorldObjectManager.h"

using namespace Plato;

/*
	Expected behaviour: See dust2 map, it generates materials automatically from an mtl file.
*/
namespace {
}

Dust2Scene::Dust2Scene() :
    BenchmarkScene(__FUNCTION__) {} // Set the benchmark scenes name

void Dust2Scene::Setup()
{
    // Create skybox
    (SkyboxPrefab()).Instantiate();

    const std::string assetsDir = "../Scenes/Dust2/assets";

    // Load dust2 materials
    MTLParser mtlParser;
    mtlParser.ParseMtl(assetsDir+"/dust2.mtl", assetsDir);

    // Load dust2 model
    OBJParser OBJParser;
    dust2Mesh = new Mesh;
    *dust2Mesh = OBJParser.ParseObj(assetsDir+"/dust2.obj", true);

    // Create the dust2 map world object
    WorldObject* dust2Wo = WorldObjectManager::NewWorldObject("dust2 map");
    // Can leave material unset, because it should be set within the mesh itself, tris-by-tris, defined by the mtl file.
    dust2Wo->AddComponent<Components::MeshRenderer>(
        dust2Mesh,
        nullptr
    );
    dust2Wo->transform->Scale(Vector3d(1,1,1)* 20);

    // Create a new main camera
    camera = WorldObjectManager::NewWorldObject("benchmark main camera")
        ->AddComponent<Components::Camera>(90, 0.001, 10);
    camera->SetAsMainCamera();
    camera->transform->SetPosition(Vector3d(-99.3422, 63.5574, 210.063));
    camera->transform->SetRotation(Vector3d(17.86, 60.58, 15.67));

    // Create player light
    WorldObjectManager::NewWorldObject("player-light", camera->transform)
        ->AddComponent<Components::PointLight>(15, Color(255, 183, 140));

    // Init the reset clock
    clock.Reset();

    return;
}

Dust2Scene::~Dust2Scene()
{
    delete dust2Mesh;
    dust2Mesh = nullptr;
}

void Dust2Scene::Update(double deltaTime)
{
    // Rotate the camera to create *some* movement
    camera->transform->Rotate(Vector3d(0,0.005 * deltaTime, 0));

    // Terminat after 15 seconds
    if (clock.GetElapsedTime().AsSeconds() >= 15.0) {
        Stop();
    }
}

