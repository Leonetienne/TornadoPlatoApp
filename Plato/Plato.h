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
	
	Mesh mesh_floatingTriangle;
	MeshRenderer* mr_floatingTriangle;

	Mesh mesh_floor;
	MeshRenderer* mr_floor;

	Camera* camera;
};

