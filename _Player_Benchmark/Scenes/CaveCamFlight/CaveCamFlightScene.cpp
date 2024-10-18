#include "CaveCamFlightScene.h"
#include "../../../Plato/Color.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/ResourceManager.h"
#include "../../../Plato/WorldObjectManager.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace Plato;

/*
 * Camera flight through cave
*/

CaveCamFlightScene::CaveCamFlightScene() :
    BenchmarkScene(__FUNCTION__) {}; //Set the benchmark scenes name

void CaveCamFlightScene::Setup()
{
    const std::string assetsDir = "../Scenes/CaveCamFlight/assets";

	// Load mesh files
	ResourceManager::LoadMeshFromObj("cave", assetsDir + "/cave.obj");
	ResourceManager::LoadMeshFromObj("cones", assetsDir + "/cones.obj");
	ResourceManager::LoadMeshFromObj("lamps", assetsDir + "/lamps.obj");
	ResourceManager::LoadMeshFromObj("lamps-cable", assetsDir + "/lamp-cable.obj");
	ResourceManager::LoadMeshFromObj("plants", assetsDir + "/plants.obj");
	ResourceManager::LoadMeshFromObj("water", assetsDir + "/water.obj");
	Mesh* lampPoints = ResourceManager::LoadMeshFromObj("lamp-points", assetsDir + "/lamp-points.obj");
	Mesh* cameraPath = ResourceManager::LoadMeshFromObj("camera-path", assetsDir + "/camera-path.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("cave", assetsDir + "/cave-color.png.bmp");
	ResourceManager::LoadTextureFromBmp("cones", assetsDir + "/cones-color.png.bmp");
	ResourceManager::LoadTextureFromBmp("lamps", assetsDir + "/lamp-color.png.bmp");
	ResourceManager::LoadTextureFromBmp("lamps-cable", assetsDir + "/cable-texture.png.bmp");
	ResourceManager::LoadTextureFromBmp("plants", assetsDir + "/vine-texture.png.bmp");
	ResourceManager::LoadTextureFromBmp("water", assetsDir + "/water-color.png.bmp");

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

    // Create and place lamp pointlights
    std::stringstream ss;
    int i = 0;
    for (const Vector3d& v : lampPoints->v_vertices) {
        ss.str("");
        ss << "Hanging Light - " << i++;
        Components::LightSource* ls = WorldObjectManager::NewWorldObject(ss.str())
            ->AddComponent<Components::PointLight>(5, Color(100, 65, 31));
        ls->SetSoftness(0.1);
        ls->transform->SetPosition(v);
    }

    // Populate camera waypoints
    cameraWaypoints = cameraPath->v_vertices;

    // Create a new main camera (screw the existing one)
    camera = WorldObjectManager::NewWorldObject("benchmark main camera")
        ->AddComponent<Components::Camera>(90, 0.001, 10);
    camera->SetAsMainCamera();
    camera->transform->SetPosition(cameraWaypoints[nextCameraWaypoint]);

    return;
}

void CaveCamFlightScene::Update(double deltaTime)
{
    // If we have a next waypoint, move the camera to it, and make it look at it
    constexpr double cameraSpeed = 250;
    if (nextCameraWaypoint != std::string::npos) {
        // Are we close enough to the next waypoint? If yes, set it
        if (camera->transform->GetPosition().Similar(cameraWaypoints[nextCameraWaypoint], 1)) {
            SetNextCameraWaypoint();
        }

        // Move the camera
        // Smoothely apply the movement via gradual lerp
        const Vector3d direction = (cameraWaypoints[nextCameraWaypoint] - camera->transform->GetPosition()).Normalize();
        const Vector3d targetPosition = camera->transform->GetPosition() + direction * cameraSpeed;
        camera->transform->SetPosition(camera->transform->GetPosition().Lerp(targetPosition, 0.0001 * deltaTime));

        // Rotate the camera
        const double pitch = std::asin(direction.z) * Rad2Deg + 90;
        const double yaw = std::atan2(direction.z, direction.x) * Rad2Deg + 90;  // XZ plane (left/right)

        // Smoothely apply the rotation via gradual lerp
        camera->transform->SetRotation(
            camera->transform->GetRotation().Lerp(
                Eule::Quaternion(Vector3d(pitch, yaw, 0)),
                0.00001 * cameraSpeed * deltaTime
            )
        );
    }
}

void CaveCamFlightScene::SetNextCameraWaypoint()
{
    nextCameraWaypoint++;
    if (nextCameraWaypoint >= cameraWaypoints.size()) {
        nextCameraWaypoint = std::string::npos;
        Stop();
    }
}

