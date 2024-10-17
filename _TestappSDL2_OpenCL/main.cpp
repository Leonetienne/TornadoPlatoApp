#include "../Scenes/Fun/Dust2/Dust2Scene.h"
#include "../Scenes/Fun/MC_House/MC_HouseScene.h"
#include "../Prefabs/FPSCamera/FPSCameraPrefab.h"
#include "../Frontend/SDL2RenderWindow.h"
#include "../Frontend/RenderWindow.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Tornado/PixelBuffer.h"
#include "../Plato/Clock.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define GPU

int main(int argc, char* argv[]) {
    const Vector2i resolution(800, 600);

    // Set up an SDL2 window
    TorGL::PixelBuffer<3> pixelBuffer(resolution);
    SDL2RenderWindow renderWindow(resolution, "OpenCL TEST", &pixelBuffer);
    RenderWindow* pRenderWindow = (RenderWindow*)(&renderWindow); // Dumb workaroud for lambda function

    // Init plato event manager
    Input::EventManager::Init();

    // Set up an FPS camera
    (FPSCameraPrefab(FPSCameraPrefab::CONTROL_TYPE::LOOK_MOUSE)).Instantiate();


    // Register logic for plato to close the render window
    Input::EventManager::RegisterReverseEventCallback(
        Input::REVERSE_EVENT_CALLBACK::EXIT, 
		[pRenderWindow](const std::vector<double>& params)
		{
			pRenderWindow->Close();
		}
    );

    // Set up the main loop
    Clock frametimer;
    double frametime = 1000.0 / 60.0;

    #ifdef GPU
    //Test OpenCL
    cl_int CL_err = CL_SUCCESS;
    cl_uint numPlatf = 0;

    CL_err = clGetPlatformIDs(0, NULL, &numPlatf);

    if (CL_err == CL_SUCCESS)
        printf("%u platform(s) found\n", numPlatf);
    else
        printf("clGetPlatformIDs(%i)\n", CL_err);

    // Get the platform ID
    cl_platform_id platform = NULL;
    CL_err = clGetPlatformIDs(1, &platform, NULL);
    if (CL_err != CL_SUCCESS) {
        printf("clGetPlatformIDs error %i\n", CL_err);
        exit(1);
    }

    // Get a GPU device
    cl_device_id device = NULL;
    CL_err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if (CL_err != CL_SUCCESS) {
        printf("clGetDeviceIDs error %i\n", CL_err);
        exit(1);
    }

    // Print gpu stats
    cl_ulong mem_size;
    char device_name[128];
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
    printf("GPU Name: %s\n", device_name);
    printf("Available GPU memory: %lu bytes\n", mem_size);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &CL_err);
    if (CL_err != CL_SUCCESS) {
        printf("clCreateContext error %i\n", CL_err);
        exit(1);
    }

    // Create a command queue
    // For OpenCL 1.2, use clCreateCommandQueue
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &CL_err);
    if (CL_err != CL_SUCCESS) {
        printf("clCreateCommandQueue error %i\n", CL_err);
        exit(1);
    }

    const char* kernelSrc = R"(
        __kernel void process_pixel(
            __global uchar* pixelBuffer, 
            int resolution_x, 
            int resolution_y,
            __global uint* fc
        ) {
            int x = get_global_id(0); // X-coordinate
            int y = get_global_id(1); // Y-coordinate

            if (x >= resolution_x || y >= resolution_y) {
                return;
            }

            int index = (resolution_x * y + x) * 3;

            __global uchar* texel = &pixelBuffer[index];
            uchar r, g, b;

            r = (x+y-*fc)%255;
            g = (x%((y+*fc)+1))%255;
            b = ((y+*fc)%(x+1))%255;

            texel[0] = r;
            texel[1] = g;
            texel[2] = b;


            // This would be more performant
            // but i want to set an example on how to
            // update cpu-calculated values in vram the render pipeline...
            //if (x == 0 && y == 0) {
                // Only one thread updates the frameCounter
                //(*frameCounter)++;
            //}
        }
    )";

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSrc, NULL, &CL_err);
    if (CL_err != CL_SUCCESS) {
        printf("clCreateProgramWithSource error %i\n", CL_err);
        exit(1);
    }

    // Build the program
    CL_err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (CL_err != CL_SUCCESS) {
        // Get and print the build error log
        size_t logSize;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
        char* log = (char*)malloc(logSize);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
        printf("clBuildProgram error:\n%s\n", log);
        free(log);
        exit(1);
    }

    // Define the pixel buffer size (resolution_x * resolution_y * 3)
    size_t buffer_size = pixelBuffer.GetSizeofBuffer();

    // Create the pixel buffer on the device
    cl_mem pixelBufferCL = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, &CL_err);
    if (CL_err != CL_SUCCESS) {
        printf("clCreateBuffer error %i\n", CL_err);
        exit(1);
    }

    // Copy the pixel buffer to the device
    CL_err = clEnqueueWriteBuffer(queue, pixelBufferCL, CL_TRUE, 0, buffer_size, pixelBuffer.GetRawData(), 0, NULL, NULL);
    if (CL_err != CL_SUCCESS) {
        printf("clEnqueueWriteBuffer error %i\n", CL_err);
        exit(1);
    }

    // Allocate frameCounter buffer in VRAM
    cl_mem frameCounterCL = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), NULL, &CL_err);
    if (CL_err != CL_SUCCESS) {
        printf("clCreateBuffer for frameCounter error %i\n", CL_err);
        exit(1);  // Exit if buffer allocation fails
    }

    // Create the kernel
    cl_kernel kernel = clCreateKernel(program, "process_pixel", &CL_err);
    if (CL_err != CL_SUCCESS) {
        printf("clCreateKernel error %i\n", CL_err);
        exit(1);
    }

    // Set kernel arguments
    CL_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pixelBufferCL);
    CL_err |= clSetKernelArg(kernel, 1, sizeof(int), &resolution.x);
    CL_err |= clSetKernelArg(kernel, 2, sizeof(int), &resolution.y);
    CL_err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &frameCounterCL);
    if (CL_err != CL_SUCCESS) {
        printf("clSetKernelArg error %i\n", CL_err);
        exit(1);
    }

    // Define the global work size (resolution_x by resolution_y)
    size_t globalSize[] = { (size_t)resolution.x, (size_t)resolution.y };
    #endif GPU

    unsigned int frameCounter = 0;
    while (renderWindow.IsRunning()) {
        // Poll the SDL2 windows events
        renderWindow.PollEvents();
        // Digest plato events
        Input::EventManager::Digest();

        // Draw a fun image using CPU
        // About 170 fps (without redrawing the window)
        #ifndef GPU
        for (std::size_t y = 0; y < resolution.y; y++) {
            for (std::size_t x = 0; x < resolution.x; x++) {
                uint8_t* texel = pixelBuffer.GetRawData() + ((resolution.x * y + x) * 3);
                uint8_t& r = texel[0];
                uint8_t& g = texel[1];
                uint8_t& b = texel[2];

                // Animated version
                // Shorthand for prettier formula... The compiler will optimize that away...
                const unsigned int fc = frameCounter;
                r = (x+y-fc)%255;
                g = (x%((y+fc)+1))%255;
                b = ((y+fc)%(x+1))%255;
            }
        }
        #endif

        // Draw with GPU
        // About 1300 FPS
        // Enqueue OpenCL kernel for processing pixelBuffer
        #ifdef GPU
        CL_err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
        if (CL_err != CL_SUCCESS) {
            printf("clEnqueueNDRangeKernel error %i\n", CL_err);
            break;
        }

        // Wait for the kernel to complete
        clFinish(queue);

        // Read the results from the GPU
        size_t buffer_size = pixelBuffer.GetSizeofBuffer();
        CL_err = clEnqueueReadBuffer(queue, pixelBufferCL, CL_TRUE, 0, buffer_size, pixelBuffer.GetRawData(), 0, NULL, NULL);
        if (CL_err != CL_SUCCESS) {
            printf("clEnqueueReadBuffer error %i\n", CL_err);
            break;
        }
        #endif

        ///renderWindow.RedrawWindow();
        // Calculate how long the frame took to display
        frametime = frametimer.GetElapsedTime().AsMilliseconds();
        frametimer.Reset();
    
        std::cout << "FPS: " << 1000.0 / frametime << std::endl;

        // Increment frame counter and copy to vram
        frameCounter++;

        #ifdef GPU
        CL_err = clEnqueueWriteBuffer(queue, frameCounterCL, CL_TRUE, 0, sizeof(int), &frameCounter, 0, NULL, NULL);
        if (CL_err != CL_SUCCESS) {
            printf("clEnqueueWriteBuffer error %i\n", CL_err);
        }
        #endif
    }

    // Clean up
    #ifdef GPU
    clReleaseMemObject(pixelBufferCL);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    #endif

    WorldObjectManager::Free();
    ResourceManager::Free();

    return 0;
}

