#include "Test__Yubi.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"
#include "../Plato/PointLight.h"
#include "../Eule/Eule/Vector3.h"
#include "../Plato/Color.h"
#include <iostream>
#include <sstream>

using namespace Plato;

/*
	Expected behaviour: We are loading two models (the monkey, and the hat) from one obj file
	No materials / textures supplied
*/

namespace {
    inline WorldObject* createYubi(Vector3d pos) {
        std::stringstream ss; ss << rand();
        WorldObject* yubi = WorldObjectManager::NewWorldObject("yubi-" + ss.str());
        yubi->AddComponent<Components::MeshRenderer>(
            ResourceManager::FindMesh("yubi"),
            ResourceManager::FindMaterial("yubi")
        );
        yubi->transform->Move(pos);
        yubi->transform->Scale(Vector3d::one * 0.1);
        yubi->transform->Rotate(Vector3d(90, 0, 0));
        return yubi;
    }
}

Test__Yubi::Test__Yubi() : TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Configure camera
	ResourceManager::LoadMeshFromObj("yubi", "assets/yubi_lowpoly.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("yubi", "assets/yubi.bmp");


	// Create materials
	ResourceManager::NewMaterial("yubi")->texture = ResourceManager::FindTexture("yubi");

	// Create yubikey
    int line = -1;
    constexpr int linesize = 20;
    for (unsigned int i = 0; i < 50; i++) {
        // Every tenth key initiates a new line
        if (i%linesize == 0) {
            line++;
        }

        WorldObject* yubi = createYubi(
            Vector3d(-20, -5, -7) + // Base position (first yubikey)
            Vector3d(((i%linesize) * 2), 0, line*-5) // Offset position (for every other yubikey)
        );

        // On every second key, adjust rotation and position
        if (i%2) {
            yubi->transform->SetRotation(Vector3d(90, 0, 180));
            yubi->transform->Move(Vector3d(1.8, 0, -2.5));
        }
    }

    // Create lights
    Components::PointLight* leftLight = WorldObjectManager::NewWorldObject("left-light")->AddComponent<Components::PointLight>(1500, Color(255, 200, 150));
    Components::PointLight* rightLight = WorldObjectManager::NewWorldObject("right-light")->AddComponent<Components::PointLight>(1500, Color(150, 200, 255));
    leftLight->transform->Move(Vector3d(-5, 5, 5) * 3);
    rightLight->transform->Move(Vector3d(5, 5, 5) * 3);

	return;
}

void Test__Yubi::Update(double deltaTime)
{
	return;
}

void Test__Yubi::Render(Renderer* renderer)
{
	return;
}

