cmake_minimum_required(VERSION 3.16)
project(TestappSDL2OpenCL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast -funroll-loops")

# Find SDL2
find_package(SDL2 REQUIRED)

# Find OpenCL
find_package(OpenCL REQUIRED)


file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)

add_executable(
    TestappSDL2OpenCL.out

    ${Eule}
    ${Tornado}
    ${Plato}

    ../Frontend/RenderWindow.cpp
    ../Frontend/SDL2RenderWindow.cpp

    ../Scenes/Fun/MC_House/MC_HouseScene.cpp
    ../Scenes/Fun/MC_Furnace/MC_FurnaceScene.cpp
    ../Scenes/Fun/Dust2/Dust2Scene.cpp

    main.cpp
)

# Include SDL2 directories
target_include_directories(TestappSDL2OpenCL.out PRIVATE ${SDL2_INCLUDE_DIRS})
# Include OpenCL directories
target_include_directories(TestappSDL2OpenCL.out PRIVATE ${OpenCL_INCLUDE_DIRS}) 

# Link SDL2 to the executable
target_link_libraries(TestappSDL2OpenCL.out PRIVATE SDL2::SDL2 OpenCL::OpenCL)

# optional? Sets the version to 1.0.0, did according to docs from OpenCL
target_compile_definitions(TestappSDL2OpenCL.out PRIVATE CL_TARGET_OPENCL_VERSION=100)

# Link assets dir into build dir
add_custom_command(
    TARGET TestappSDL2OpenCL.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

