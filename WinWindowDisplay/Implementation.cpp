#include "Implementation.h"
#include "renderres.h"
#include <math.h>

AppImplementation::AppImplementation(std::size_t width, std::size_t height)
{
	bgrPixelBuffer = new unsigned char[width * height * 3];
	plato = new Plato();

	return;
}

AppImplementation::~AppImplementation()
{
	delete[] bgrPixelBuffer;
	delete plato;

	plato = nullptr;
	bgrPixelBuffer = nullptr;
}

void AppImplementation::Tick()
{
	plato->Update();

	for (std::size_t x = 0; x < RENDER_WIDTH; x++)
		for (std::size_t y = 0; y < RENDER_HEIGHT; y++)
		{
			uint8_t& r = bgrPixelBuffer[3 * (y * RENDER_WIDTH + x) + 2];
			uint8_t& g = bgrPixelBuffer[3 * (y * RENDER_WIDTH + x) + 1];
			uint8_t& b = bgrPixelBuffer[3 * (y * RENDER_WIDTH + x) + 0];

			r = *plato->GetPixelBuffer()->GetPixel(Vector2i(x, y), 0);
			g = *plato->GetPixelBuffer()->GetPixel(Vector2i(x, y), 1);
			b = *plato->GetPixelBuffer()->GetPixel(Vector2i(x, y), 2);
		}

	return;
}

unsigned const char* AppImplementation::GetBGRPixelBuffer()
{
	return bgrPixelBuffer;
}
