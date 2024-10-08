cmake_minimum_required(VERSION 3.16)
project(Eule)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_EULE_NO_INTRINSICS_)

add_library(Eule
        Collider.cpp
        Math.cpp
        Matrix4x4.cpp
        Quaternion.cpp
        Random.cpp
        TrapazoidalPrismCollider.cpp
        Vector2.cpp
        Vector3.cpp
        Vector4.cpp
)
