cmake_minimum_required(VERSION 3.16)
project(_Testapp_FPS_Headless)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_EULE_NO_INTRINSICS_)

include_directories(..)

FILE(GLOB Eule ../Eule/Eule/*.cpp)
FILE(GLOB Tornado ../Tornado/*.cpp)
FILE(GLOB Plato ../Plato/*.cpp)

add_executable(
        __Testapp_FPS_Headless

        ${Eule}
        ${Tornado}
        ${Plato}

        TestFixture.cpp
        Test__FPS.cpp
        Test__Yubi.cpp
        Test__Cube.cpp
        Test__MC.cpp
        main.cpp
)

