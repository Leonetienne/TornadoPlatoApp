#include "Plato.h"
#include <Windows.h>

Plato::Plato()
{
	tornado = new Tornado(Vector2i(800, 600), 24);

	rd.a.pos_worldSpace = Vector3d(0, -0.3, 5);
	rd.b.pos_worldSpace = Vector3d(1, -2, 5);
	rd.c.pos_worldSpace = Vector3d(-1, -2, 5);

	rd.a.vertexColor = Color::red;
	rd.b.vertexColor = Color::green;
	rd.c.vertexColor = Color::blue;
	
	worldObject = WorldObjectManager::NewWorldObject();
	camera = WorldObjectManager::NewWorldObject();

	return;
}

Plato::~Plato()
{
	delete tornado;

	tornado = nullptr;

	return;
}

void Plato::Update()
{
	static double fov = 90;
	const double shiftmod = GetAsyncKeyState(VK_LSHIFT) ? 5 : 1;

	if (GetAsyncKeyState('A'))
		camera->GetTransform()->Move(Vector3d(-1, 0, 0) * 0.1 * shiftmod);
	if (GetAsyncKeyState('D'))
		camera->GetTransform()->Move(Vector3d(1, 0, 0) * 0.1 * shiftmod);

	if (GetAsyncKeyState('W'))
		camera->GetTransform()->Move(Vector3d(0, 0,  1) * 0.1 * shiftmod);
	if (GetAsyncKeyState('S'))
		camera->GetTransform()->Move(Vector3d(0, 0, -1) * 0.1 * shiftmod);

	if (GetAsyncKeyState('Q'))
		camera->GetTransform()->Move(Vector3d(0,  1, 0) * 0.1 * shiftmod);
	if (GetAsyncKeyState('E'))
		camera->GetTransform()->Move(Vector3d(0, -1, 0) * 0.1 * shiftmod);

	if (GetAsyncKeyState('1'))
		fov += 1 * shiftmod;
	if (GetAsyncKeyState('2'))
		fov -= 1 * shiftmod;

	if (GetAsyncKeyState('Y'))
		camera->GetTransform()->Rotate(Quaternion::FromEuler(Vector3d(0, 10,0) * 0.1 * shiftmod));
	if (GetAsyncKeyState('X'))
		camera->GetTransform()->Rotate(Quaternion::FromEuler(Vector3d(0, -10,0) * 0.1 * shiftmod));

	RenderTriangle3D ard = rd;
	ard.a.pos_worldSpace = rd.a.pos_worldSpace * worldObject->GetTransform()->GetLocalTransformationMatrix();
	ard.b.pos_worldSpace = rd.b.pos_worldSpace * worldObject->GetTransform()->GetLocalTransformationMatrix();
	ard.c.pos_worldSpace = rd.c.pos_worldSpace * worldObject->GetTransform()->GetLocalTransformationMatrix();

	// Camera transformation
	ard.a.pos_worldSpace -= camera->GetTransform()->GetPosition();
	ard.b.pos_worldSpace -= camera->GetTransform()->GetPosition();
	ard.c.pos_worldSpace -= camera->GetTransform()->GetPosition();
	ard.a.pos_worldSpace *= camera->GetTransform()->GetRotation().Inverse().ToRotationMatrix();
	ard.b.pos_worldSpace *= camera->GetTransform()->GetRotation().Inverse().ToRotationMatrix();
	ard.c.pos_worldSpace *= camera->GetTransform()->GetRotation().Inverse().ToRotationMatrix();

	ard.a.vertexColor = rd.a.vertexColor;
	ard.b.vertexColor = rd.b.vertexColor;
	ard.c.vertexColor = rd.c.vertexColor;

	ProjectionProperties prop(Vector2i(800, 600), fov, 0.001, 10000);

	Matrix4x4 worldMatrix;

	worldMatrix[0] = { 1, 0,  0, 0 };
	worldMatrix[1] = { 0, 1,  0, 0 };
	worldMatrix[2] = { 0, 0, -1, 0 };
	worldMatrix[3] = { 0, 0,  0, 0 };
	tornado->BeginFrame();
	tornado->RegisterRender(&ard);
	tornado->Render(prop, worldMatrix);

	return;
}

PixelBuffer<3>* Plato::GetPixelBuffer() const
{
	return tornado->GetPixelBuffer();
}
