#pragma once
#include <cstddef>
// Cross platform implementations of syscalls.
// Code is in cpp file to avoid heavy includes in files using this

namespace TorGL
{
	void cpSleep(std::size_t ms);
}
