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
* `Prefabs/`: Prefab definitions. So-to-speak, predefined more complex game object structures
* `Scenes/`: Scenes definitions.
* `StaticMeshClasses/`: If a mesh is simple enough to be hardcoded, it can be defined here.
* `LegacyCode/`: Old implementations such as a windows window

## How2cook
Every build needs cmake. Install that. 
Some need SDL2 for presentation aswell. Install that. (`libsdl2-dev`)
Some (will soon) need OpenCL for non-laggy running. Install that. (`opencl-headers ocl-icd-opencl-dev`)

### How2cook terminal player
1. `cd _TestappTERM`
2. `cmake -B build`
3. `cd build`
4. `make`
5. `./_TestappTERM.out`

### How2cook SDL2 player
1. Haf SDL2 installed. 
2. `cd _TestappSDL2`
3. `cmake -B build`
4. `cd build`
5. `make`
6. `./_TestappSDL2.out`

### How2cook OpenCL with SDL2 player
1. Haf OpenCL installed 
2. Haf SDL2 installed 
3. `cd _TestappSDL2_OpenCL`
4. `cmake -B build`
5. `cd build`
6. `make`
7. `./_TestappSDL2OpenCL.out`

### How2cook visual acceptance tests
1. `cd _Test_Acceptance_Tests`
2. Select test to run in main.cpp
3. `cmake -B build`
4. `cd build`
5. `make`
6. `./AcceptanceTest.out`

### How2cook unit tests
1. `cd _Test_All`
2. `cmake -B build`
3. `cd build`
4. `make`
5. `./Test_All.out`

## How2SetScene
If you want to set a scene you have to write
`Scene* scene = new (SceneName)`
in the `main.cpp` file in order to initiate it. Scenes can be found at 
`./Scenes/`. 
