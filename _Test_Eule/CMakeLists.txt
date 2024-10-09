cmake_minimum_required(VERSION 3.16)
project(Tests_Eule)

set(CMAKE_CXX_STANDARD 17)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Eule_Test ./*.cpp)

add_executable(
    Test_Eule.out
    ${Eule}
    ${Eule_Test}
)

