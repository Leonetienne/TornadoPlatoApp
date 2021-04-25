#include "Plato.h"
#include <Windows.h>
#include "Camera.h"

#include "bmplib.h"
using namespace BMPlib;

class Rotator : Component
{
public:
	void Update(double)
	{
		if (GetAsyncKeyState(VK_SPACE))
			transform->Rotate(Quaternion(Vector3d(
				0.5 * speed,
				1.0 * speed,
				2.0 * speed
			)));


		if (GetAsyncKeyState('K'))
			transform->SetRotation(Quaternion(Vector3d::zero));

		return;
	}

private:
	Rotator(WorldObject* worldObject, double speed)
		:
		Component(worldObject)
	{
		this->speed = speed;
	}
	double speed;

	friend WorldObject;
};

// Just for the testing lolz
void CreateCubeFractal(const Mesh* mesh, const Material* material, std::size_t depth = 2)
{
	std::vector<WorldObject*> addTo;
	addTo.push_back(WorldObjectManager::NewWorldObject());
	addTo[0]->GetTransform()->Move(Vector3d(4, -0, 6));
	addTo[0]->GetTransform()->Rotate(Vector3d(90, 0, 0)); // To make the image upside-up
	addTo[0]->AddTag("rotating_cube");
	addTo[0]->AddComponent<MeshRenderer>(mesh, material);
	addTo[0]->AddComponent<Rotator>((((rand() % 20) / 10.0) - 1.0) * 0.5);

	const std::size_t max_its = (std::size_t)pow(6, depth) + (std::size_t)pow(7, depth-1);
	std::size_t its = 0;

	for (std::size_t i = 0; i < addTo.size(); i++)
	{
		if (its > max_its)
			return;

		// Six sides + center
		for (std::size_t f = 0; f < 6; f++)
		{
			WorldObject* newCube = WorldObjectManager::NewWorldObject("", addTo[i]->GetTransform());
			newCube->AddTag("rotating_cube");
			newCube->AddComponent<MeshRenderer>(mesh, material);
			newCube->AddComponent<Rotator>((((rand() % 20) / 10.0) - 1.0) * 0.5);
	
			// Use these offsets for the edges
			static const Vector3d edgePositions[76] = {
				Vector3d( 1,  0,  0),
				Vector3d(-1,  0,  0),
				Vector3d( 0,  1,  0),
				Vector3d( 0, -1,  0),
				Vector3d( 0,  0,  1),
				Vector3d( 0,  0, -1)
			};
				
			newCube->GetTransform()->SetScale(Vector3d::one * 0.45);
			newCube->GetTransform()->Move(edgePositions[f] * 2);
			newCube->GetTransform()->Rotate(Vector3d(90, 0, 0)); // To make the image upside-up

			addTo.push_back(newCube);
			its++;
		}
	}

	return;
}


Transform* floor3;

Plato::Plato()
{
	renderer = new Renderer(Vector2i(800*2, 600*2));

	mesh_floor.v_vertices = {
		Vector3d(-1, 0, 1),
		Vector3d( 1, 0, 1),
		Vector3d(-1, 0,-1),
		Vector3d( 1, 0,-1)
	};
	mesh_floor.uv_vertices = {
		Vector2d(0,0),
		Vector2d(1,0),
		Vector2d(0,1),
		Vector2d(1,1)
	};
	mesh_floor.normals = {
		Vector3d(0,0,0)
	};
	mesh_floor.tris = {
		{0,0,0},
		{1,1,0},
		{3,3,0},

		{3,3,0},
		{2,2,0},
		{0,0,0}
	};

	mesh_coob.v_vertices = {
		Vector3d(-1, -1,  1),
		Vector3d(-1,  1,  1),
		Vector3d(-1, -1, -1),
		Vector3d(-1,  1, -1),
		Vector3d( 1, -1,  1),
		Vector3d( 1,  1,  1),
		Vector3d( 1, -1, -1),
		Vector3d( 1,  1, -1)
	};
	mesh_coob.uv_vertices = {
		Vector2d(0.625, 0.00),
		Vector2d(0.375, 0.25),
		Vector2d(0.375, 0.00),
		Vector2d(0.625, 0.25),
		Vector2d(0.375, 0.50),
		Vector2d(0.625, 0.50),
		Vector2d(0.375, 0.75),
		Vector2d(0.625, 0.75),
		Vector2d(0.375, 1.00),
		Vector2d(0.125, 0.75),
		Vector2d(0.125, 0.50),
		Vector2d(0.875, 0.50),
		Vector2d(0.625, 1.00),
		Vector2d(0.875, 0.75)
	};
	mesh_coob.normals = {
		Vector3d(-1,  0,  0),
		Vector3d( 0,  0, -1),
		Vector3d( 1,  0,  0),
		Vector3d( 0,  0,  1),
		Vector3d( 0, -1,  0),
		Vector3d( 0,  1,  0)
	};
	mesh_coob.tris = {
		{1,  0, 0}, { 2,  1, 0}, {0,  2, 0},
		{3,  3, 1}, { 6,  4, 1}, {2,  1, 1},
		{7,  5, 2}, { 4,  6, 2}, {6,  4, 2},
		{5,  7, 3}, { 0,  8, 3}, {4,  6, 3},
		{6,  4, 4}, { 0,  9, 4}, {2,  10, 4},
		{3, 11, 5}, { 5,  7, 5}, {7,  5, 5},
		{1,  0, 0}, { 3,  3, 0}, {2,  1, 0},
		{3,  3, 1}, { 7,  5, 1}, {6,  4, 1},
		{7,  5, 2}, { 5,  7, 2}, {4,  6, 2},
		{5,  7, 3}, { 1, 12, 3}, {0,  8, 3},
		{6,  4, 4}, { 4,  6, 4}, {0,  9, 4},
		{3, 11, 5}, { 1, 13, 5}, {5,  7, 5}
	};

	// Protocode load texture from file
	BMP bmp;
	bmp.Read("../Plato/Cube_thielen_gitignore_.bmp");
	bmp.ConvertTo(BMP::COLOR_MODE::RGBA);
	Texture* tx_coob = new Texture(Vector2i((int)bmp.GetWidth(), (int)bmp.GetHeight()));
	tx_coob->GetPixelBuffer().Refit(bmp.GetPixelBuffer(), Vector2i((int)bmp.GetWidth(), (int)bmp.GetHeight()));
	dummyMat.texture = tx_coob;

	camera_yPivot = WorldObjectManager::NewWorldObject()->GetTransform();
	camera_yPivot->worldObject->SetId("cam_y_pivot");
	camera = WorldObjectManager::NewWorldObject("main_camera", camera_yPivot)->AddComponent<Camera>(Vector2i(800*2, 600*2), 90, 0.0001, 10000);
	renderer->SetMainCamera(camera);
	
	mr_coob = WorldObjectManager::NewWorldObject()->AddComponent<MeshRenderer>(&mesh_coob, &dummyMat);
	mr_coob->transform->Move(Vector3d::forward * 6 + Vector2d::down * 1.5);
	mr_coob->worldObject->AddComponent<Rotator>(1);
	
	mr_coob2 = WorldObjectManager::NewWorldObject("cube", mr_coob->transform)->AddComponent<MeshRenderer>(&mesh_coob, &dummyMat);
	mr_coob2->transform->SetScale(Vector3d::one * 0.5);
	mr_coob2->transform->Move(Vector3d(0, 2, 0));
	//mr_coob2->worldObject->CreateComponent<Rotator>(1);

	CreateCubeFractal(&mesh_coob, &dummyMat, 2);

	return;
}

Plato::~Plato()
{
	delete renderer;

	renderer = nullptr;

	return;
}

void Plato::Update()
{
	const double shiftmod = GetAsyncKeyState(VK_LSHIFT) ? 5 : 1;

	WorldObjectManager::DeleteFlaggedObjects();
	WorldObjectManager::CallHook__Update(0);

	// PLAYER CONTROL
	if (GetAsyncKeyState('A'))
		camera_yPivot->Move(camera_yPivot->GetRotation() * Vector3d::left * 0.1 * shiftmod);
	if (GetAsyncKeyState('D'))
		camera_yPivot->Move(camera_yPivot->GetRotation() * Vector3d::right * 0.1 * shiftmod);

	if (GetAsyncKeyState('W'))
		camera_yPivot->Move(camera_yPivot->GetRotation() * Vector3d::forward * 0.1 * shiftmod);
	if (GetAsyncKeyState('S'))
		camera_yPivot->Move(camera_yPivot->GetRotation() * Vector3d::backward * 0.1 * shiftmod);

	if (GetAsyncKeyState('Q'))
		camera_yPivot->Move(Vector3d::up * 0.1 * shiftmod);
	if (GetAsyncKeyState('E'))
		camera_yPivot->Move(Vector3d::down * 0.1 * shiftmod);

	

	if (GetAsyncKeyState('P'))
		mr_coob2->transform->SetParent(mr_coob->transform);

	if (GetAsyncKeyState('U'))
	{
		mr_coob2->transform->SetParent(nullptr);
		for (WorldObject* wo : WorldObjectManager::FindObjectsByTag("rotating_cube"))
			wo->GetTransform()->SetParent(nullptr);
	}

	if (GetAsyncKeyState(VK_RIGHT))
		mr_floor->transform->Rotate(Quaternion(Vector3d::up * 0.5 * shiftmod));
	if (GetAsyncKeyState(VK_LEFT))
		mr_floor->transform->Rotate(Quaternion(Vector3d::down * 0.5 * shiftmod));

	if (GetAsyncKeyState(VK_UP))
		mr_floor->transform->Rotate(Quaternion(Vector3d::right * 0.5 * shiftmod));
	if (GetAsyncKeyState(VK_DOWN))
		mr_floor->transform->Rotate(Quaternion(Vector3d::left * 0.5 * shiftmod));

	if (GetAsyncKeyState(VK_NUMPAD8))
		mr_floor->transform->Move(Vector3d::forward * 0.5 * shiftmod);
	if (GetAsyncKeyState(VK_NUMPAD5))
		mr_floor->transform->Move(Vector3d::backward * 0.5 * shiftmod);
	if (GetAsyncKeyState(VK_NUMPAD4))
		mr_floor->transform->Move(Vector3d::left * 0.5 * shiftmod);
	if (GetAsyncKeyState(VK_NUMPAD6))
		mr_floor->transform->Move(Vector3d::right * 0.5 * shiftmod);

	if (GetAsyncKeyState('Y'))
		camera_yPivot->Rotate(Quaternion(Vector3d(0, -2, 0 ) * shiftmod));
	if (GetAsyncKeyState('X'))
		camera_yPivot->Rotate(Quaternion(Vector3d(0, 2, 0 ) * shiftmod));
	if (GetAsyncKeyState('C'))
		camera->transform->Rotate(Quaternion(Vector3d(2, 0, 0) * shiftmod));
	if (GetAsyncKeyState('F'))
		camera->transform->Rotate(Quaternion(Vector3d(-2, 0, 0) * shiftmod));

	renderer->BeginFrame();
	WorldObjectManager::CallHook__Render(renderer);
	renderer->Render();

	return;
}

const PixelBuffer<3>* Plato::GetPixelBuffer() const
{
	return renderer->GetPixelBuffer();
}
