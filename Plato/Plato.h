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
	
	Mesh mesh_floatingTriangle;
	Material mat_floatingTriangle;
	MeshRenderer* mr_floatingTriangle;

	Camera* camera;
};

