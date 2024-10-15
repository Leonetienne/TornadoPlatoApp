#pragma once
#include "Renderer.h"
#include <string>

namespace  Plato {
    class Scene
    {
    public:
        Scene(const std::string& testName = "");

        virtual void Update(double frametime) {};
        virtual void LateUpdate(double frametime) {};
        virtual void Render(Renderer* renderer) {};

        const std::string& GetSceneName() const;

    private:
        std::string sceneName;
    };
}

