#include "Implementation.h"
#include <math.h>

AppImplementation::AppImplementation(std::size_t width, std::size_t height)
{
	bgrPixelBuffer = new unsigned char[width * height * 3];

	return;
}

AppImplementation::~AppImplementation()
{
	delete[] bgrPixelBuffer;

	bgrPixelBuffer = nullptr;
}

void AppImplementation::Tick()
{
	static double t = 0;
	t += 0.5;
	for (std::size_t x = 0; x < 800; x++)
		for (std::size_t y = 0; y < 600; y++)
		{
			bgrPixelBuffer[3 * (y * 800 + x) + 2] = (unsigned char)(tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255); // r
			bgrPixelBuffer[3 * (y * 800 + x) + 1] = (unsigned char)(tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255); // g
			bgrPixelBuffer[3 * (y * 800 + x) + 0] = (unsigned char)(tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255); // b
		}

	return;
}

unsigned const char* AppImplementation::GetBGRPixelBuffer()
{
	return bgrPixelBuffer;
}
