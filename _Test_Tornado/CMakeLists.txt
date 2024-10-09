cmake_minimum_required(VERSION 3.16)
project(Tests_Tornado)

set(CMAKE_CXX_STANDARD 17)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Tornado_Test ./*.cpp)

add_executable(
        Test_Tornado.out
        ${Eule}
        ${Tornado}
        ${Tornado_Test}
)

