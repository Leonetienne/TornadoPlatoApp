#include "Plato.h"
#include <Windows.h>
#include "Camera.h"

Transform* floor3;

Plato::Plato()
{
	renderer = new Renderer(Vector2i(800, 600));

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
		{1,  0, 0}, { 2,  1, 0}, {0, 2, 0},
		{3,  3, 1}, { 6,  4, 1}, {2, 1, 1},
		{7,  5, 2}, { 4,  6, 2}, {6, 4, 2},
		{5,  7, 3}, { 0,  8, 3}, {4, 6, 3},
		{6,  4, 4}, { 0,  9, 4}, {2, 9, 4},
		{3, 10, 5}, { 5,  7, 5}, {7, 5, 5},
		{1,  0, 0}, { 3,  3, 0}, {2, 1, 0},
		{3,  3, 1}, { 7,  5, 1}, {6, 4, 1},
		{7,  5, 2}, { 5,  7, 2}, {4, 6, 2},
		{5,  7, 3}, { 1, 11, 3}, {0, 8, 3},
		{6,  4, 4}, { 4,  6, 4}, {0, 9, 4},
		{3, 10, 5}, { 1, 13, 5}, {5, 7, 5}
	};

	camera = WorldObjectManager::NewWorldObject()->CreateComponent<Camera>(Vector2i(800, 600), 90, 0.0001, 10000);
	renderer->SetMainCamera(camera);
	
	mr_coob = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_coob, &dummyMat);
	mr_coob->transform->Move(Vector3d::forward * 6 + Vector2d::down * 2);

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

	if (GetAsyncKeyState('A'))
		camera->transform->Move(camera->transform->GetRotation() * Vector3d::left * 0.1 * shiftmod);
	if (GetAsyncKeyState('D'))
		camera->transform->Move(camera->transform->GetRotation() * Vector3d::right * 0.1 * shiftmod);

	if (GetAsyncKeyState('W'))
		camera->transform->Move(camera->transform->GetRotation() * Vector3d::forward * 0.1 * shiftmod);
	if (GetAsyncKeyState('S'))
		camera->transform->Move(camera->transform->GetRotation() * Vector3d::backward * 0.1 * shiftmod);

	if (GetAsyncKeyState('Q'))
		camera->transform->Move(Vector3d::up * 0.1 * shiftmod);
	if (GetAsyncKeyState('E'))
		camera->transform->Move(Vector3d::down * 0.1 * shiftmod);

	if (GetAsyncKeyState('1'))
		camera->SetFov(camera->GetFov() - 1 * shiftmod);
	if (GetAsyncKeyState('2'))
		camera->SetFov(camera->GetFov() + 1 * shiftmod);

	//if (GetAsyncKeyState('P'))
	//	mr_floatingTriangle->transform->SetParent(mr_floor->transform);
	//
	//if (GetAsyncKeyState('U'))
	//	floor3->SetParent(nullptr);


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
		camera->transform->Rotate(Quaternion::FromEuler(Vector3d::up * 1 * shiftmod));
	if (GetAsyncKeyState('X'))
		camera->transform->Rotate(Quaternion::FromEuler(Vector3d::up * -1 * shiftmod));
	
	renderer->BeginFrame();
	WorldObjectManager::CallHook__Render(renderer);
	renderer->Render();

	return;
}

const PixelBuffer<3>* Plato::GetPixelBuffer() const
{
	return renderer->GetPixelBuffer();
}
