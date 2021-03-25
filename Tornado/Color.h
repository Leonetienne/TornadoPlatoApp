#pragma once
#include <cstdint>

struct Color
{
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0;

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;

	static const Color black;
	static const Color gray;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color cyan;
	static const Color pink;
	static const Color white;
};
