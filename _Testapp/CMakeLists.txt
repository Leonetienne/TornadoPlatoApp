cmake_minimum_required(VERSION 3.16)
project(Testapp)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_EULE_NO_INTRINSICS_)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)

add_executable(
        Testapp.out

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


# Link assets dir into build dir
add_custom_command(
    TARGET Testapp.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

