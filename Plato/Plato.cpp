#include "Plato.h"
#include <Windows.h>
#include "Camera.h"

Transform* floor3;

Plato::Plato()
{
	renderer = new Renderer(Vector2i(800, 600));

	mesh_floatingTriangle.v_vertices = {
		Vector3d(0, 1.7, 0),
		Vector3d(1, 0, 0),
		Vector3d(-1, 0, 0)
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

	camera = WorldObjectManager::NewWorldObject()->CreateComponent<Camera>(Vector2i(800, 600), 90, 0.0001, 10000);
	mr_floor = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat);
	//mr_floatingTriangle = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floatingTriangle, &dummyMat);

	renderer->SetMainCamera(camera);

	mr_floor->transform->SetScale(Vector3d::one * 20);
	mr_floor->transform->SetPosition(Vector3d::down * 2);

	//mr_floatingTriangle->transform->SetPosition({ 0, -2, 5 });

	Transform* floor1 = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat)->transform;
	Transform* floor2 = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat)->transform;
	floor3 = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat)->transform;
	Transform* floor4 = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat)->transform;
	Transform* floor5 = WorldObjectManager::NewWorldObject()->CreateComponent<MeshRenderer>(&mesh_floor, &dummyMat)->transform;
	
	floor5->SetParent(floor4);
	floor4->SetParent(floor3);
	floor3->SetParent(floor2);
	floor2->SetParent(floor1);
	floor1->SetParent(mr_floor->transform);
	
	floor1->SetPosition(Vector3d::up * 0.25 );
	floor1->SetScale(Vector3d::one * 0.5 );
	
	floor2->SetPosition(Vector3d::up * 0.25);
	floor2->SetScale(Vector3d::one * 0.5);
	
	floor3->SetPosition(Vector3d::up * 0.25);
	floor3->SetScale(Vector3d::one * 0.5);
	
	floor4->SetPosition(Vector3d::up * 0.25);
	floor4->SetScale(Vector3d::one * 0.5);
	
	floor5->SetPosition(Vector3d::up * 0.25);
	floor5->SetScale(Vector3d::one * 0.5);

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

	if (GetAsyncKeyState('P'))
		mr_floatingTriangle->transform->SetParent(mr_floor->transform);

	if (GetAsyncKeyState('U'))
		floor3->SetParent(nullptr);


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
