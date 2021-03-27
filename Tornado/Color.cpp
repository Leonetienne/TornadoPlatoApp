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

Color::Color(double r, double g, double b, double a)
	:
	r{ r },
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

void Color::LerpSelf(const Color& other, double t)
{
	double in_t = 1.0 - t;

	r = in_t*r + t*other.r;
	g = in_t*g + t*other.g;
	b = in_t*b + t*other.b;
	a = in_t*a + t*other.a;

	return;
}

Color Color::Lerp(const Color& other, double t) const
{
	double in_t = 1.0 - t;

	return Color(
		in_t * r + t * other.r,
		in_t * g + t * other.g,
		in_t * b + t * other.b,
		in_t * a + t * other.a
	);
}

Color Color::Lerp(const Color& a, const Color& b, double t)
{
	double in_t = 1.0 - t;

	return Color(
		in_t * a.r + t * b.r,
		in_t * a.g + t * b.g,
		in_t * a.b + t * b.b,
		in_t * a.a + t * b.a
	);
}
