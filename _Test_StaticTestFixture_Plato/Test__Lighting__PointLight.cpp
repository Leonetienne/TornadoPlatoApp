#pragma once
#include "Test__Lighting__PointLight.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Keyboard.h"
#include "../Plato/Math.h"
#include "Rotator.h"
#include <iostream>


/*
	Expected behaviour: 
	The monkey should only be lit from the front.
	The light should cycle through a color wheel.
	The rest should be dark.
*/

Test__Lighting__PointLight::Test__Lighting__PointLight() :
	TestFixture(__FUNCTION__) // Set the test fixtures name
{
	// Create plane mesh
	Mesh* mPlane = ResourceManager::NewMesh("plane");
	mPlane->v_vertices = {
		Vector3d(-1, 0, -1),
		Vector3d(1, 0, -1),
		Vector3d(1, 0, 1),
		Vector3d(-1, 0, 1)
	};
	mPlane->uv_vertices = {
		Vector2d(0, 0),
		Vector2d(1, 0),
		Vector2d(1, 1),
		Vector2d(0, 1)
	};
	mPlane->normals = {
		Vector3d::up
	};
	mPlane->tris = {
		{0, 0, 0},
		{2, 2, 0},
		{1, 1, 0},
		{3, 3, 0},
		{2, 2, 0},
		{0, 0, 0},
	};

	// Load mesh files
	ResourceManager::LoadMeshFromObj("monke", "monke.obj");

	// Load texture files
	ResourceManager::LoadTextureFromBmp("monke", "monke.bmp");
	ResourceManager::LoadTextureFromBmp("floor", "texture_floor__gitignore_.bmp");


	// Create materials
	ResourceManager::NewMaterial("monke")->texture = ResourceManager::FindTexture("monke");
	ResourceManager::NewMaterial("floor")->texture = ResourceManager::FindTexture("floor");

	// Create plane
	woPlane = WorldObjectManager::NewWorldObject("plane");
	woPlane->AddComponent<MeshRenderer>(mPlane, ResourceManager::FindMaterial("floor"));
	woPlane->transform->Scale(Vector3d::one * 40);
	woPlane->transform->Move(Vector3d::down * 2 + Vector3d::backward * -10);

	// Create monkey
	WorldObject* monke = WorldObjectManager::NewWorldObject("monke");
	monke->AddComponent<MeshRenderer>(
		ResourceManager::FindMesh("monke"),
		ResourceManager::FindMaterial("monke")
		);
	monke->transform->Move(Vector3d::backward * 5);
	//monke->AddComponent<Rotator>(Vector3d(0, 1.0, 0));

	// Create point light
	WorldObject* woLight;
	//woLight; = WorldObjectManager::NewWorldObject("point light");
	//PointLight* pointLight = woLight->AddComponent<PointLight>(100, Color(0, 130, 255));
	//pointLight->transform->Move(Vector3d::left * 4 + Vector3d::backward * 3);
	//
	//woLight = WorldObjectManager::NewWorldObject("point light");
	//pointLight = woLight->AddComponent<PointLight>(100, Color(255, 130, 0));
	//pointLight->transform->Move(Vector3d::right * 4 + Vector3d::backward * 3);

	rgbParent = WorldObjectManager::NewWorldObject("rgb parent")->transform;

	woLight = WorldObjectManager::NewWorldObject("rgb light", rgbParent);
	rgbLight = woLight->AddComponent<PointLight>(20, Color(255, 255, 255));
	rgbLight->transform->Move(Vector3d::up * 4 + Vector3d::backward * 13);

	return;
}

void Test__Lighting__PointLight::Update(double deltaTime)
{
	static double absTime = 0;
	absTime += deltaTime;

	//woPlane->transform->Rotate(Quaternion(Vector3d::right * 0.01 * deltaTime));

	//rgbLight->SetSoftness(Math::Oscillate(0, 0.5, absTime, 0.002));
	//rgbLight->SetIntensity(Math::Oscillate(5, 40, absTime, 0.001));

	rgbParent->Rotate(Quaternion(Vector3d::up * 0.05 * deltaTime));

	//UpdateColorCycle(deltaTime);

	return;
}

void Test__Lighting__PointLight::UpdateColorCycle(double deltaTime)
{
	static Color lightColor = Color::red;
	static int c = 2;
	static bool up = true;

	static double speed = 50.0;

	switch (c)
	{
	case 0:
		lightColor.r += (up ? 1 : -1) * speed;
		lightColor.r = Math::Clamp(lightColor.r, 0, 255);
		break;

	case 1:
		lightColor.g += (up ? 1 : -1) * speed;
		lightColor.g = Math::Clamp(lightColor.g, 0, 255);
		break;

	case 2:
		lightColor.b += (up ? 1 : -1) * speed;
		lightColor.b = Math::Clamp(lightColor.b, 0, 255);
		break;
	}

	rgbLight->SetColor(lightColor);

	switch (c)
	{
	case 0:
		c = 1;

		if (Math::Similar(lightColor.r, 255))
			up = false;
		else if (Math::Similar(lightColor.r, 0))
			up = true;

		break;

	case 1:
		c = 2;

		if (Math::Similar(lightColor.g, 255))
			up = false;
		else if (Math::Similar(lightColor.g, 0))
			up = true;

		break;

	case 2:
		c = 0;

		if (Math::Similar(lightColor.b, 255))
			up = false;
		else if (Math::Similar(lightColor.b, 0))
			up = true;

		break;
	}


	return;
}
