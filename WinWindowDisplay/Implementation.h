#pragma once
#include <cstddef>
#include "../Plato/Vector.h"
#include "../Plato/Plato.h"

class AppImplementation
{
public:
	AppImplementation(std::size_t width, std::size_t height);
	~AppImplementation();

	void Tick();
	unsigned const char* GetBGRPixelBuffer();

private:
	uint8_t* bgrPixelBuffer; // BitBlt uses bgr.

	Plato* plato;
};
