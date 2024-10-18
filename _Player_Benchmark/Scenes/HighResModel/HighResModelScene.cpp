#include "HighResModelScene.h"
#include "../../../Prefabs/Skybox/SkyboxPrefab.h"
#include "../../../Plato/WorldObjectManager.h"
#include "../../../Plato/ResourceManager.h"
#include "../../../Plato/MeshRenderer.h"
#include "../../../Plato/Material.h"
#include "../../../Plato/Camera.h"

using namespace Plato;

/*
 * Displays a high resolution character model, spinning, for five seconds.
*/


HighResModelScene::HighResModelScene():
    BenchmarkScene(__FUNCTION__) {}; //Set the benchmark scenes name

void HighResModelScene::Setup()
{
    // Create skybox
    (SkyboxPrefab()).Instantiate();

    const std::string assetsDir = "../Scenes/HighResModel/assets";

	// Load mesh files
	ResourceManager::LoadMeshFromObj("bk", assetsDir + "/bk.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("bk", assetsDir + "/bk.bmp");

	// Create materials
	Material* matBricks = ResourceManager::NewMaterial("bk");
	matBricks->texture = ResourceManager::FindTexture("bk");

	// Create world objects
    woBk = WorldObjectManager::NewWorldObject("bk")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("bk"),
            ResourceManager::FindMaterial("bk")
    )->worldObject;

    // Create a new main camera
    camera = WorldObjectManager::NewWorldObject("benchmark main camera")
        ->AddComponent<Components::Camera>(90, 0.001, 10);
    camera->SetAsMainCamera();
    camera->transform->SetPosition(Vector3d(0, 0.5,1));
    camera->transform->Rotate(Vector3d(15, 0, 0));

    // Init the reset clock
    clock.Reset();

    return;
}

void HighResModelScene::Update(double deltaTime)
{
    // Rotate the highpoly mesh
    woBk->transform->Rotate(Vector3d(0, 0.1 * deltaTime, 0));

    // Terminat after 15 seconds
    if (clock.GetElapsedTime().AsSeconds() >= 15.0) {
        Stop();
    }
}

void HighResModelScene::Render(Renderer* renderer)
{
}

void HighResModelScene::Teardown()
{
}
