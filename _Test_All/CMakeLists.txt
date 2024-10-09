cmake_minimum_required(VERSION 3.16)
project(Tests_All)

set(CMAKE_CXX_STANDARD 17)

file(GLOB Eule ../Eule/*.cpp)
file(GLOB Tornado ../Tornado/*.cpp)
file(GLOB Plato ../Plato/*.cpp)
file(GLOB Eule_Test ../_Test_Eule/*.cpp)
file(GLOB Tornado_Test ../_Test_Tornado/*.cpp)
file(GLOB Plato_Test ../_Test_Plato/*.cpp)

list(FILTER Eule_Test EXCLUDE REGEX ".*/main.cpp")
list(FILTER Tornado_Test EXCLUDE REGEX ".*/main.cpp")
list(FILTER Plato_Test EXCLUDE REGEX ".*/main.cpp")

add_executable(
    Test_All.out

    ${Eule}
    ${Tornado}
    ${Plato}

    ${Eule_Test}
    ${Tornado_Test}
    ${Plato_Test}
    main.cpp
)

