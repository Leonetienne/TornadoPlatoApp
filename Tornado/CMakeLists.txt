cmake_minimum_required(VERSION 3.16)
project(Tornado)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_EULE_NO_INTRINSICS_)

include_directories(..)

FILE(GLOB Eule ../Eule/Eule/*.cpp)

add_library(
        Tornado

        ${Eule}

        BackfaceCullingEngine.cpp
        BarycentricInterpolationEngine.cpp
        ClippingEngine.cpp
        Color.cpp
        CrossPlatformSyscalls.cpp
        DrawingEngine.cpp
        InterRenderTriangle.cpp
        InterRenderVertex.cpp
        LightingEngine.cpp
        Material.cpp
        ProjectionEngine.cpp
        ProjectionProperties.cpp
        RenderLightSource.cpp
        RenderPointLight.cpp
        Texture.cpp
        Tornado.cpp
        WorkerPool.cpp
)
