#pragma once
#include "WorldObjectManager.h"
#include "Renderer.h"

class Plato
{
public:
	Plato();
	~Plato();

	void Update();
	const PixelBuffer<3>* GetPixelBuffer() const;

private:
	Renderer* renderer;
	Material dummyMat;

	Mesh mesh_floor;
	MeshRenderer* mr_floor;

	Mesh mesh_coob;
	MeshRenderer* mr_coob;
	MeshRenderer* mr_coob2;

	Transform* camera_yPivot;
	Camera* camera;
};

