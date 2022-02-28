cmake_minimum_required(VERSION 3.16)
project(Plato)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_EULE_NO_INTRINSICS_)

include_directories(..)

FILE(GLOB Eule ../Eule/Eule/*.cpp)
FILE(GLOB Tornado ../Tornado/*.cpp)

add_library(
        Plato

        ${Eule}
        ${Tornado}

        Application.cpp
        Camera.cpp
        Clock.cpp
        Collider.cpp
        Component.cpp
        EventManager.cpp
        Keyboard.cpp
        LightSource.cpp
        Mesh.cpp
        MeshRenderer.cpp
        Mouse.cpp
        OBJParser.cpp
        Plato.cpp
        PointLight.cpp
        Renderer.cpp
        ResourceManager.cpp
        Transform.cpp
        TrapazoidalPrismCollider.cpp
        WorldObject.cpp
        WorldObjectManager.cpp
)
