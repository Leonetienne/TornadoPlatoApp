#include "HighResModelScene.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/MeshRenderer.h"
#include "../Plato/Material.h"
#include "../Plato/Camera.h"
#include "BenchmarkScene.h"

using namespace Plato;

/*
 * Displays a high resolution character model, spinning, for five seconds.
*/


HighResModelScene::HighResModelScene():
    BenchmarkScene(__FUNCTION__) {};

void HighResModelScene::Setup()
{
    const std::string assetsDir = "./assets/high-res-model";

	// Load mesh files
	ResourceManager::LoadMeshFromObj("bk", assetsDir + "/bk.obj");
	ResourceManager::LoadMeshFromObj("skybox", "./assets/general-assets/skybox.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("bk", assetsDir + "/bk.bmp");
	ResourceManager::LoadTextureFromBmp("skybox", "./assets/general-assets/example_skybox.bmp");

	// Create materials
	Material* matBricks = ResourceManager::NewMaterial("bk");
	matBricks->texture = ResourceManager::FindTexture("bk");
	matBricks->noShading = true;
	Material* matSkybox = ResourceManager::NewMaterial("skybox");
	matSkybox->texture = ResourceManager::FindTexture("skybox");
	matSkybox->noShading = true;

	// Create world objects
    woBk = WorldObjectManager::NewWorldObject("bk")
        ->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("bk"),
            ResourceManager::FindMaterial("bk")
    )->worldObject;

	// Create skybox
	WorldObject* skybox = WorldObjectManager::NewWorldObject("skybox");
	skybox->AddComponent<Components::MeshRenderer>(
		ResourceManager::FindMesh("skybox"),
		ResourceManager::FindMaterial("skybox")
		);
	skybox->transform->Scale(Vector3d::one * 50);

    // Create a new main camera (screw the existing one)
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

    // Terminat after five seconds
    if (clock.GetElapsedTime().AsSeconds() >= 5.0) {
        Stop();
    }
}

void HighResModelScene::Render(Renderer* renderer)
{
}

void HighResModelScene::Teardown()
{
}
