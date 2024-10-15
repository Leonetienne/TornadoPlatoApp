#pragma once
#include "../../../Plato/Scene.h"
#include "../../../Plato/Mesh.h"

class Dust2Scene : public Plato::Scene
{
public:
	Dust2Scene();
	~Dust2Scene();

	void Update(double deltaTime) override;

private:
    Plato::Mesh* dust2Mesh;
};

