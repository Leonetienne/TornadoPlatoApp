#pragma once
#include <cstddef>

class AppImplementation
{
public:
	AppImplementation(std::size_t width, std::size_t height);
	~AppImplementation();

	void Tick();
	unsigned const char* GetBGRUPixelBuffer();

private:
	unsigned char* bgruPixelBuffer; // BitBlt uses bgru. u is useless, because it's unused.
};
