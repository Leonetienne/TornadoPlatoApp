#pragma once
#include "Transform.h"

namespace Plato {
    class Prefab
    {
    public:
        virtual void Instantiate(Plato::Transform* parent = nullptr) {};
    };
}

