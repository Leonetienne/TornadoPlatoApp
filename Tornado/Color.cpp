#include "Color.h"

const Color Color::black	{   0,   0,   0, 255 };
const Color Color::gray		{ 128, 128, 128, 255 };
const Color Color::red		{ 255,   0,   0, 255 };
const Color Color::green	{   0, 255,   0, 255 };
const Color Color::blue		{   0,   0, 255, 255 };
const Color Color::yellow	{ 255, 255,   0, 255 };
const Color Color::cyan		{   0, 255, 255, 255 };
const Color Color::pink		{ 255,   0, 255, 255 };
const Color Color::white	{ 255, 255, 255, 255 };

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: r{ r },
	  g{ g },
	  b{ b },
	  a{ a }
{
	return;
}

bool Color::operator==(const Color& other) const
{
	return
		(r == other.r) &&
		(g == other.g) &&
		(b == other.b) &&
		(a == other.a);
}

bool Color::operator!=(const Color& other) const
{
	return !operator==(other);
}
