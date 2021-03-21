#include "Implementation.h"
#include <math.h>

AppImplementation::AppImplementation(std::size_t width, std::size_t height)
{
	bgruPixelBuffer = new unsigned char[width * height * 4];

	Vector2d vd2;
	Vector3d vd3;
	Vector4d vd4;

	Vector2i vi2;
	Vector3i vi3;
	Vector4i vi4;
	
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
			bgruPixelBuffer[(y * 4 * 800 + x * 4) + 2] = (unsigned char)(tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255); // r
			bgruPixelBuffer[(y * 4 * 800 + x * 4) + 1] = (unsigned char)(tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255); // g
			bgruPixelBuffer[(y * 4 * 800 + x * 4) + 0] = (unsigned char)(tan((x + t) / 40.0) * tan((y + t) / 40.0) * 255); // b
		}

	return;
}

unsigned const char* AppImplementation::GetBGRUPixelBuffer()
{
	return bgruPixelBuffer;
}
