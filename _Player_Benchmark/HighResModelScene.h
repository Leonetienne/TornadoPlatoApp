#pragma once
#include "BenchmarkScene.h"
#include "../Plato/Clock.h"

using namespace Plato;

class HighResModelScene : public BenchmarkScene
{
public:
    void Setup() override;
	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;
	void Teardown() override;
    std::string GetName() const override;

private:
    WorldObject* woBk;
    Clock clock;
};

