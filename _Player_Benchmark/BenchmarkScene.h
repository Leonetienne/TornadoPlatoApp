#pragma once
#include "../Plato/Camera.h"
#include "../Plato/Scene.h"

using namespace Plato;

class BenchmarkScene : public Scene
{
public:
	virtual void Update(double deltaTime) {};
	virtual void Render(Renderer* renderer) {};

    bool GetIsRunning() const;

    // DONT override this! This sets running to true!
    void Start();

    // Feel free to override this
    virtual void Teardown() {};

protected:
    BenchmarkScene(const std::string& sceneName);

    // Will stop the scene and make the player go to the next
    void Stop();

    // Feel free to override this
    // This is where you should create your scene!
    virtual void Setup() {};

    Components::Camera* camera;
    bool running = false;
};

