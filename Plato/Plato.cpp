#include "Plato.h"
#include <Windows.h>
#include "Camera.h"

Plato::Plato()
{
	renderer = new Renderer(Vector2i(800, 600));

	mesh_floatingTriangle.v_vertices = {
		Vector3d(0, -0.3, 5),
		Vector3d(1, -2, 5),
		Vector3d(-1, -2, 5)
	};
	mesh_floatingTriangle.uv_vertices = {
		Vector2d(0,0)
	};
	mesh_floatingTriangle.normals = {
		Vector3d(0,0,0)
	};
	mesh_floatingTriangle.tris = {
		{0,0,0},
		{1,0,0},
		{2,0,0}
	};

	mesh_floor.v_vertices = {
		Vector3d(-100, -10, 100),
		Vector3d( 100, -10, 100),
		Vector3d(-100, -10,-100),
		Vector3d( 100, -10,-100)
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

	camera = WorldObjectManager::NewWorldObject()->CreateComponent<Camera>(Vector2i(800, 600), 90, 0.0001, 10000);
	mr_floor = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat);
	mr_floatingTriangle = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floatingTriangle, &dummyMat);

	renderer->SetMainCamera(camera);

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
	static double fov = 90;
	const double shiftmod = GetAsyncKeyState(VK_LSHIFT) ? 5 : 1;

	WorldObjectManager::DeleteFlaggedObjects();
	WorldObjectManager::CallHook__Update(0);

	if (GetAsyncKeyState('A'))
		camera->transform->Move(Vector3d(-1, 0, 0) * 0.1 * shiftmod);
	if (GetAsyncKeyState('D'))
		camera->transform->Move(Vector3d(1, 0, 0) * 0.1 * shiftmod);

	if (GetAsyncKeyState('W'))
		camera->transform->Move(Vector3d(0, 0,  1) * 0.1 * shiftmod);
	if (GetAsyncKeyState('S'))
		camera->transform->Move(Vector3d(0, 0, -1) * 0.1 * shiftmod);

	if (GetAsyncKeyState('Q'))
		camera->transform->Move(Vector3d(0,  1, 0) * 0.1 * shiftmod);
	if (GetAsyncKeyState('E'))
		camera->transform->Move(Vector3d(0, -1, 0) * 0.1 * shiftmod);

	if (GetAsyncKeyState('1'))
		fov += 1 * shiftmod;
	if (GetAsyncKeyState('2'))
		fov -= 1 * shiftmod;

	if (GetAsyncKeyState('Y'))
		camera->transform->Rotate(Quaternion::FromEuler(Vector3d(0, 10,0) * 0.1 * shiftmod));
	if (GetAsyncKeyState('X'))
		camera->transform->Rotate(Quaternion::FromEuler(Vector3d(0, -10,0) * 0.1 * shiftmod));
	
	renderer->BeginFrame();
	WorldObjectManager::CallHook__Render(renderer);
	renderer->Render();

	return;
}

const PixelBuffer<3>* Plato::GetPixelBuffer() const
{
	return renderer->GetPixelBuffer();
}
