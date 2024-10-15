#pragma once
#include "Renderer.h"
#include <string>

namespace  Plato {
    class Scene
    {
    public:
        virtual void Update(double frametime) {};
        virtual void LateUpdate(double frametime) {};
        virtual void Render(Renderer* renderer) {};

        const std::string& GetSceneName() const;

    protected:
        Scene(const std::string& sceneName);

    private:
        std::string sceneName;
    };
}

