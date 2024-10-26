cmake_minimum_required(VERSION 3.16)
project(Testapp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Ofast -funroll-loops -Wall")

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)

add_executable(
    Testapp.out

    ${Eule}
    ${Tornado}
    ${Plato}

    ../Frontend/RenderWindow.cpp
    ../Frontend/TerminalBLockCharacterRenderWindow.cpp

    ../Scenes/Fun/MC_House/MC_HouseScene.cpp
    ../Scenes/Fun/MC_Furnace/MC_FurnaceScene.cpp
    ../Scenes/Fun/Dust2/Dust2Scene.cpp

    main.cpp
)

# Link assets dir into build dir
add_custom_command(
    TARGET Testapp.out POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Linking assets directory to build directory."
)

