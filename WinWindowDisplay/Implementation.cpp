#include "Implementation.h"
#include <math.h>

AppImplementation::AppImplementation(std::size_t width, std::size_t height)
{
	bgruPixelBuffer = new unsigned char[width * height * 4];
	
	return;
}

AppImplementation::~AppImplementation()
{
	delete[] bgruPixelBuffer;

	bgruPixelBuffer = nullptr;
}

void AppImplementation::Tick()
{
	static double t = 0;
	t += 0.5;
	for (std::size_t x = 0; x < 800; x++)
		for (std::size_t y = 0; y < 600; y++)
		{
			bgruPixelBuffer[(y * 4 * 800 + x * 4) + 2] = tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255; // r
			bgruPixelBuffer[(y * 4 * 800 + x * 4) + 1] = tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255; // g
			bgruPixelBuffer[(y * 4 * 800 + x * 4) + 0] = tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255; // b
		}

	return;
}

unsigned const char* AppImplementation::GetBGRUPixelBuffer()
{
	return bgruPixelBuffer;
}
