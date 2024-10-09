cmake_minimum_required(VERSION 3.16)
project(Tests_Tornado)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_EULE_NO_INTRINSICS_)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Tornado_Test ./*.cpp)

add_executable(
        Test_Tornado.out
        ${Eule}
        ${Tornado}
        #${Torado_Test}
        Color.cpp
        Texture.cpp
        BarycentricInterpolationEngine.cpp
        WorkerPool.cpp
        main.cpp
)

