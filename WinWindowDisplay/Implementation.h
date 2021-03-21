#pragma once
#include <cstddef>
#include "../Tornado/Vector2.h"
#include "../Tornado/Vector3.h"
#include "../Tornado/Vector4.h"

class AppImplementation
{
public:
	AppImplementation(std::size_t width, std::size_t height);
	~AppImplementation();

	void Tick();
	unsigned const char* GetBGRUPixelBuffer();

private:
	unsigned char* bgrPixelBuffer; // BitBlt uses bgr.
};
