#pragma once
#include "../../BenchmarkScene.h"
#include "../../../Plato/Clock.h"

class HighResModelScene : public BenchmarkScene
{
public:
    HighResModelScene();

    void Setup() override;
	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;
	void Teardown() override;

private:
    WorldObject* woBk;
    Clock clock;
};

