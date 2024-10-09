cmake_minimum_required(VERSION 3.16)
project(TestappSDL2)

set(CMAKE_CXX_STANDARD 17)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)

add_executable(
        TestappSDL2.out

        ${Eule}
        ${Tornado}
        ${Plato}

        TestFixture.cpp
        Test__MC.cpp
        main.cpp
)

# Link assets dir into build dir
add_custom_command(
    TARGET TestappSDL2.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

