#include "Dust2Scene.h"
#include "../../../Prefabs/Skybox/SkyboxPrefab.h"
#include "../../../Plato/MTLParser.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/WorldObjectManager.h"
#include "../../../Plato/MeshRenderer.h"

using namespace Plato;

/*
	Expected behaviour: See dust2 map, it generates materials automatically from an mtl file.
*/
namespace {
}

Dust2Scene::Dust2Scene() : Scene(__FUNCTION__) // Set the test fixtures name
{
    // Create skybox
    (SkyboxPrefab()).Instantiate();

    const std::string assetsDir = "../../Scenes/Fun/Dust2/assets";

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

    // Create player light
    WorldObject* cameraWo = WorldObjectManager::FindObjectById("main_camera");
    if (!cameraWo) {
        throw std::runtime_error(std::string("Attempted to initialize scene ") + GetSceneName() + " without having a any camera loaded! But that is required!");
    }
    WorldObjectManager::NewWorldObject("player-light", cameraWo->transform)
        ->AddComponent<Components::PointLight>(15, Color(255, 183, 140));

    // Position main camera
    WorldObject* fpsCameraParent = WorldObjectManager::FindObjectById("main_camera_ypiv");
    if (fpsCameraParent) {
        fpsCameraParent->transform->SetPosition(Vector3d(-99.3422, 63.5574, 210.063));
        fpsCameraParent->transform->SetRotation(Vector3d(17.86, 60.58, 15.67));
    }

    return;
}

Dust2Scene::~Dust2Scene()
{
    delete dust2Mesh;
    dust2Mesh = nullptr;
}

void Dust2Scene::Update(double deltaTime)
{
    
}

