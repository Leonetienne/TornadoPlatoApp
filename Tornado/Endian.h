#pragma once
#include <cstdint>

constexpr bool IsBigEndian()
{
	return (uint8_t)(((uint8_t)0x0B << 8) | (uint8_t)0x0A) == (uint8_t)0xBA;
}
