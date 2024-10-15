cmake_minimum_required(VERSION 3.16)
project(TestappSDL2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast -funroll-loops")

# Find SDL2
find_package(SDL2 REQUIRED)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)

add_executable(
    TestappSDL2.out

    ${Eule}
    ${Tornado}
    ${Plato}

    ../Frontend/RenderWindow.cpp
    ../Frontend/SDL2RenderWindow.cpp

    ../Scenes/Fun/MC_House/MC_HouseScene.cpp

    main.cpp
)

# Include SDL2 directories
target_include_directories(TestappSDL2.out PRIVATE ${SDL2_INCLUDE_DIRS})

# Link SDL2 to the executable
target_link_libraries(TestappSDL2.out PRIVATE SDL2::SDL2)

# Link assets dir into build dir
add_custom_command(
    TARGET TestappSDL2.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

