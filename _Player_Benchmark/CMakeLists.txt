cmake_minimum_required(VERSION 3.16)
project(Player_Benchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast -funroll-loops")

# Find SDL2
find_package(SDL2 REQUIRED)

add_compile_definitions('_BENCHMARK_CONTEXT')

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)

add_executable(
    Benchmark.out

    ${Eule}
    ${Tornado}
    ${Plato}

    ../Frontend/RenderWindow.cpp
    ../Frontend/SDL2RenderWindow.cpp

    BenchmarkScene.cpp
    BenchmarkPlayer.cpp
    CaveCamFlightScene.cpp
    HighResModelScene.cpp
    main.cpp
)

# Include SDL2 directories
target_include_directories(Benchmark.out PRIVATE ${SDL2_INCLUDE_DIRS})

# Link SDL2 to the executable
target_link_libraries(Benchmark.out PRIVATE SDL2::SDL2)

# Link assets dir into build dir
add_custom_command(
    TARGET Benchmark.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

# Link python plotter dir into build dir
add_custom_command(
    TARGET Benchmark.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/dataplotter ${CMAKE_BINARY_DIR}/dataplotter
    COMMENT "Linking python plotter directory to build directory."
)
