#pragma once
#include "../../BenchmarkScene.h"
#include "../../../Plato/Clock.h"
#include "../../../Plato/Mesh.h"

class Dust2Scene : public BenchmarkScene
{
public:
	Dust2Scene();
	~Dust2Scene();

    void Setup() override;
	void Update(double deltaTime) override;

private:
    Plato::Mesh* dust2Mesh;
    Clock clock;
};

