# TornadoPlato

This is still under heavy development. It's just public for the fun in it.

## TOC
* `Eule/`: Does maths
* `Tornado/`: Does le graphics
* `Plato/`: Does le game engine things
* `Testapp*/`: Actual applications
* `_Player_Benchmark/`: Benchmark scene runner to test performance and plot performance metrics (using matplotlib in python)
* `-Test_StaticTestFixture_Plato/`: A runner for predefined testing scenes with known behaviour, to visually test behaviour.
* `Test_*/`: Catch2 unit tests
* `_TestingUtilitites/`: Helpers for catch2 unit tests
* `Frontend/`: Defines different implementations of render windows
* `HandyComponents/`: Useful in-game components, such as FPS camera controls
* `LegacyCode/`: Old implementations such as a windows window

## How2cook
1. Haf cmake installed
2. `cd _TestappTERM`
3. `cmake -B build`
4. `cd build`
5. `make`
6. `./_TestappTERM.out`

## How2cook SDL2 application
1. Haf cmake installed
2. Haf SDL2 installed
3. `cd _TestappSDL2`
4. `cmake -B build`
5. `cd build`
6. `make`
7. `./_TestappSDL2.out`

## How2cook anything else
just like the upper two...
