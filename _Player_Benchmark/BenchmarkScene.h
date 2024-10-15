#pragma once
#include "../Plato/Camera.h"

using namespace Plato;

class BenchmarkScene
{
public:
	virtual void Update(double deltaTime) {};
	virtual void Render(Renderer* renderer) {};

    bool GetIsRunning() const;

    // DONT override this! This sets running to true!
    void Start();

    // Feel free to override this
    virtual void Teardown() {};

    virtual std::string GetName() const {return "nA";};

protected:
    // Will stop the scene and make the player go to the next
    void Stop();

    // Feel free to override this
    virtual void Setup() {};

    Components::Camera* camera;
    bool running = false;
};

