# TornadoPlato

This is still under heavy development. It's just public for the fun in it.

## TOC
* `Eule/`: Does maths
* `Tornado/`: Does le graphics
* `Plato/`: Does le game engine things
* `Testapp*/`: Actual applications
* `_Player_Benchmark/`: Benchmark scene runner to test performance and plot performance metrics (using matplotlib in python)
* `_Test_Acceptance_Tests/`: A runner for predefined testing scenes with known behaviour, to visually inspect behaviour.
* `Test_*/`: Catch2 unit tests
* `_TestingUtilitites/`: Helpers for catch2 unit tests
* `Frontend/`: Defines different implementations of render windows
* `HandyComponents/`: Useful in-game components, such as FPS camera controls
* `LegacyCode/`: Old implementations such as a windows window

## How2cook
Every build needs cmake. Install that. Some need SDL2 for presentation aswell.

### How2cook terminal player
1. `cd _TestappTERM`
2. `cmake -B build`
3. `cd build`
4. `make`
5. `./_TestappTERM.out`

### How2cook SDL2 player
1. Haf SDL2 installed
2. `cd _TestappSDL2`
3. `cmake -B build`
4. `cd build`
5. `make`
6. `./_TestappSDL2.out`

### How2cook benchmarks and results
1. Haf SDL2 installed
2. Haf python installed
3. `cd _Player_Benchmark`
4. `cmake -B build`
5. `cd build`
6. `make`
7. `./Bechmark.out`
8. `cd dataplotter`
9. `source ./venv/bin/activate`
10. `python -m pip -r Reqiurements.txt`
11. `./plot-metrics.py`
12. Enjoy performance graphs as png images in `./performance-plots/`

### How2cook visual acceptance tests
1. `cd _Test_Acceptance_Tests`
2. `cmake -B build`
3. `cd build`
4. `make`
5. `./AcceptanceTest.out`

### How2cook unit tests
1. `cd _Test_All`
2. `cmake -B build`
3. `cd build`
4. `make`
5. `./Test_All.out`

