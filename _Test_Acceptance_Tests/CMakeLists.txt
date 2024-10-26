cmake_minimum_required(VERSION 3.16)
project(AcceptanceTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast -funroll-loops")

# Find SDL2
find_package(SDL2 REQUIRED)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)
file(GLOB AcceptanceTests  *.cpp)

add_executable(
    AcceptanceTests.out

    ${Eule}
    ${Tornado}
    ${Plato}

    ../Frontend/RenderWindow.cpp
    ../Frontend/SDL2RenderWindow.cpp

    ${AcceptanceTests}
)

# Include SDL2 directories
target_include_directories(AcceptanceTests.out PRIVATE ${SDL2_INCLUDE_DIRS})

# Link SDL2 to the executable
target_link_libraries(AcceptanceTests.out PRIVATE SDL2::SDL2)

# Link assets dir into build dir
add_custom_command(
    TARGET AcceptanceTests.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

# Link assets dir into build dir
add_custom_command(
    TARGET AcceptanceTests.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)
