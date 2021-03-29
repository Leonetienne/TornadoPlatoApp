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
	static double tx = 0;
	static double ty = 0;
	static double tz = 0;
	static double fov = 90;
	static double rot = 0;
	const double shiftmod = GetAsyncKeyState(VK_LSHIFT) ? 5 : 1;

	if (GetAsyncKeyState('A'))
		tx -= 0.1 * shiftmod;
	if (GetAsyncKeyState('D'))
		tx += 0.1 * shiftmod;
	if (GetAsyncKeyState('W'))
		tz += 0.1 * shiftmod;
	if (GetAsyncKeyState('S'))
		tz -= 0.1 * shiftmod;
	if (GetAsyncKeyState('Q'))
		ty -= 0.1 * shiftmod;
	if (GetAsyncKeyState('E'))
		ty += 0.1 * shiftmod;

	if (GetAsyncKeyState('1'))
		fov += 1 * shiftmod;
	if (GetAsyncKeyState('2'))
		fov -= 1 * shiftmod;

	if (GetAsyncKeyState('Y'))
		rot += 0.1 * shiftmod;
	if (GetAsyncKeyState('X'))
		rot -= 0.1 * shiftmod;

	Matrix4x4 rotMat;
	rotMat[0] = { cos(rot), 0, -sin(rot), tx };
	rotMat[1] = { 0, 1, 0, ty };
	rotMat[2] = { sin(rot), 0, cos(rot), tz };
	rotMat[3] = { 0, 0, 0, 0 };

	RenderTriangle3D ard = rd;
	ard.a.pos_worldSpace = rd.a.pos_worldSpace * rotMat;
	ard.b.pos_worldSpace = rd.b.pos_worldSpace * rotMat;
	ard.c.pos_worldSpace = rd.c.pos_worldSpace * rotMat;

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
