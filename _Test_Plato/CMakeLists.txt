cmake_minimum_required(VERSION 3.16)
project(Tests_Plato)

set(CMAKE_CXX_STANDARD 17)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)
file(GLOB Plato_Test ./*.cpp)

add_executable(
    Test_Plato.out
    ${Eule}
    ${Tornado}
    ${Plato}
    ${Plato_Test}
)

