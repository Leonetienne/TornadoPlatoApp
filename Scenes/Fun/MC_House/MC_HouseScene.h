#pragma once
#include "../../../Plato/Scene.h"
#include "../../../Plato/PointLight.h"

class MC_HouseScene : public Plato::Scene
{
public:
	MC_HouseScene();

	void Update(double deltaTime) override;

private:
    Plato::Components::PointLight* flashlight;

    void ToggleFlashlight();
    void TurnFlashlightOn();
    void TurnFlashlightOff();
};

